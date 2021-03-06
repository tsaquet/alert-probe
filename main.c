/* 
 * Alert - Probe is a collect probe, part of the Alert software
 * Copyright (C) 2013 Florent Poinsaut, Thomas Saquet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

/* Documentation tag for Doxygen
 */

/*! @mainpage Alert Probe
 * 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <addonLibManager.h>
#include <addonLibMemory.h>

/* To init Log */
#include "log.h"
/* To init struct Conf */
#include "conf.h"
/* To init struct FormatParams */
#include "format.h"
/* To init struct SenderParams */
#include "sender.h"
/* To init struct SenderParams */
#include "signals.h"



/* Probe Names */
#define PRODUCT_NAME "ECHOES Alert - Probe"
#define APP_NAME "ea-probe"
/* Probe Version */
#define VERSION "0.1.0"
/* Conf Repository */
#ifdef NDEBUG
    #define CONF_DIR "/opt/echoes-alert/probe/etc/probe.conf"
#else
    #define CONF_DIR "./conf/probe.conf"
#endif

/**
 * Main function
 * @param argc Number of arguments passed to the program
 * @param argv Array of arguments passed to the program
 * @param envp Array of environment variables passed to the program
 * @return Exit status
 */
int main(int argc, char** argv, char **envp)
{
    /*
     *Initialization
     */

    /* Probe Configuration initialisation */
    Conf conf = CONF_INITIALIZER;

    /* Log Params initialisation */
    /*TODO: Add logFile in conf param */
    LogParams logParams = {
        g_get_host_name(),
        APP_NAME,
        getpid(),
        "/var/log/echoes-alert/probe.log"
    };

    /* Signal number */
    int signum = 0;

    /* Thread identifiers initialisation */
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;

    /* Addons Manager thread params initialisation */
    AddonsMgrParams addonsMgrParams = ADDON_PARAMS_INITIALIZER;

    /* Queues initialisation */
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        g_get_host_name(),
        APP_NAME,
        conf.token,
        &conf.probeID,
        &conf.transportMsgVersion,
        getpid(),
        NULL
    };

    /* Format thread initilisation */
    FormatParams formatParams = {
        &addonsMgrParams.collectQueue,
        &sdElementQueue,
        &signum
    };

    /* Sender thread initilisation */
    SenderParams senderParams = {
        &sdElementQueue,
        conf.engineFQDN,
        &conf.enginePort,
        0,
        &conf.transportProto,
        &signum
    };

    ListOfAllPointersToFree listOfAllPointersToFree = NULL;

    /* Help message and version */
    if (argc > 1)
    {
        if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
        {
            printf("%s %s\n", PRODUCT_NAME, VERSION);
        }
        else
        {
            printf(
                   "Usage:\n"
                   "\t%s [OPTION...]\n\n"
                   "%s options\n"
                   "\t-h or --help\tPrint this message.\n"
                   "\t-v or --version\tPrint %s version.\n",
                   APP_NAME,
                   PRODUCT_NAME,
                   PRODUCT_NAME
                   );
        }
        return EXIT_SUCCESS;
    }

#ifdef NDEBUG
    g_log_set_handler(
                      G_LOG_DOMAIN,
                      G_LOG_LEVEL_MASK,
                      log2File,
                      (gpointer) & logParams
                      );
#else
    g_log_set_handler(
                      G_LOG_DOMAIN,
                      G_LOG_LEVEL_MASK,
                      log2Console,
                      (gpointer) & logParams
                      );
#endif

    /* Daemonization */
#ifdef NDEBUG
    if (chdir("/") != 0)
    {
        g_critical("%s", strerror(errno));
        return EXIT_FAILURE;
    }
    if (fork() != 0)
        exit(EXIT_SUCCESS);
    setsid();
    if (fork() != 0)
        exit(EXIT_SUCCESS);
#endif

    g_message(
              "[origin enterpriseId=\"40311\" software=\"%s\" swVersion=\"%s\"] (re)start",
              PRODUCT_NAME,
              VERSION
              );

    g_type_init();

    /* Create file that contains pid use by start-stop-daemon */
#ifdef NDEBUG
    FILE* daemonPidFile = NULL;
    char *daemonPidFilePath = "/var/run/ea-probe.pid";
    remove(daemonPidFilePath);
    daemonPidFile = fopen(daemonPidFilePath, "w");
    if (daemonPidFile != NULL)
    {
        fprintf(daemonPidFile, "%d", getpid());
        fclose(daemonPidFile);
    }
    else
    {
        g_critical("Critical: %s: %s", strerror(errno), daemonPidFilePath);
    }
#endif   

#ifndef NDEBUG
    printf("Début du chargement des conf\n");
#endif
    if (loadConf(&conf, CONF_DIR))
    {
        logStopProbe(PRODUCT_NAME, VERSION);
        return EXIT_FAILURE;
    }
#ifndef NDEBUG
    printf("Fin du chargement des conf\n");

    printf("Chargement des addons\n");
#endif
    if (addonManager(&addonsMgrParams, &listOfAllPointersToFree))
    {
        if (addonsMgrParams.threadIdentifiers->addonsThreads != NULL)
            free(addonsMgrParams.threadIdentifiers->addonsThreads);
        logStopProbe(PRODUCT_NAME, VERSION);
        return EXIT_FAILURE;
    }
#ifndef NDEBUG
    printf("Début du chargement du module Format\n");
#endif
    if (pthread_create(&threadIdentifiers.formatThread, NULL, format, (void*) &formatParams))
    {
        g_critical("Critical: %s: formatThread", strerror(errno));
        free(addonsMgrParams.threadIdentifiers->addonsThreads);
        logStopProbe(PRODUCT_NAME, VERSION);
        return EXIT_FAILURE;
    }
#ifndef NDEBUG
    printf("Fin du chargement des addons\n");
#endif

#ifndef NDEBUG
    printf("Début de l'envoi des messages\n");
#endif
    if (pthread_create(&threadIdentifiers.senderThread, NULL, sender, (void*) &senderParams))
    {
        g_critical("Critical: %s: senderThread", strerror(errno));
        free(addonsMgrParams.threadIdentifiers->addonsThreads);
        logStopProbe(PRODUCT_NAME, VERSION);
        return EXIT_FAILURE;
    }

    /* Define signals handlers */
    signalsHandler(&signum, &threadIdentifiers);

    signum = waitForShutdown();

    /* TODO: Vérifier tous les calloc et faire un fonction de cleanup
       avec entre autres les plgInfo et srcInfo etc. */

    /* Cleanup */
    free(addonsMgrParams.threadIdentifiers->addonsThreads);
    listOfAllPointersToFree_free(&listOfAllPointersToFree);

    g_message(
              "[origin enterpriseId=\"40311\" software=\"%s\" swVersion=\"%s\"] stop",
              PRODUCT_NAME,
              VERSION
              );

    if (signum == SIGHUP)
        restart(argv, envp);

    return EXIT_SUCCESS;
}

