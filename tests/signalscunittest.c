/*
 * File:   signalscunittest.c
 * Author: mla
 *
 * Created on 23 sept. 2013, 17:08:32
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "signals.h"

/*
 * CUnit Test Suite
 */

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

static void *boucleInfinie()
{
    while (1)
    {
        sleep(1);
    }
}

void testRestart()
{
    char** argv;
    char** envp;
    int result = restart(argv, envp);
    if (1 /*check result*/)
    {
        CU_ASSERT(0);
    }
}

void testSignalHandling()
{
    int signum = 0;
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    threadIdentifiers.addonsThreads = calloc(threadIdentifiers.nbAddonsThreads, sizeof (pthread_t));

    pthread_create(&threadIdentifiers.addonsThreads[0], NULL, boucleInfinie, NULL);
    threadIdentifiers.nbAddonsThreads = 1;
    signalsHandler(&signum, &threadIdentifiers);

    signalHandling(SIGTERM);
    CU_ASSERT_EQUAL(signum, SIGTERM);
    CU_ASSERT(pthread_cancel(&threadIdentifiers.addonsThreads[0]));
}

void testSignalsHandler()
{
    int signum = 0;
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    signalsHandler(&signum, &threadIdentifiers);

    kill(getpid(), SIGHUP);
    CU_ASSERT_EQUAL(signum, SIGHUP);
    kill(getpid(), SIGTERM);
    CU_ASSERT_EQUAL(signum, SIGTERM);
    kill(getpid(), SIGQUIT);
    CU_ASSERT_EQUAL(signum, SIGQUIT);
    kill(getpid(), SIGSYS);
    CU_ASSERT_EQUAL(signum, SIGSYS);
}

void testWaitForShutdown()
{
    int signum = 5;
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    signalsHandler(&signum, &threadIdentifiers);
    CU_ASSERT_EQUAL(waitForShutdown(), 5);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("signalscunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (/*(NULL == CU_add_test(pSuite, "testRestart", testRestart)) ||*/
        (NULL == CU_add_test(pSuite, "testSignalHandling", testSignalHandling)) ||
        (NULL == CU_add_test(pSuite, "testSignalsHandler", testSignalsHandler)) ||
        (NULL == CU_add_test(pSuite, "testWaitForShutdown", testWaitForShutdown)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}