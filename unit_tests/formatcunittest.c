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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#ifndef NDEBUG
    #include "CUnit/Basic.h"
#else
    #include "CUnit/Automated.h"
#endif
#include "format.h"

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

void testFormat()
{
    SDElementQueueElement* sdElementQueueElement = NULL;
    int probeID = 1;
    char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "127.0.0.1",
        "ea-probe",
        "abcd1234",
        &probeID,
        &transportMsgVersion,
        3,
        NULL
    };

    CollectQueue collectQueue = COLLECT_QUEUE_INITIALIZER;
    gint idsIDA[] = {1};
    char** values = calloc(1, sizeof (char*));
    CollectQueueElement* collectQueueElement = calloc(1, sizeof (CollectQueueElement));

    char** values2 = calloc(1, sizeof (char*));
    gint idsIDA2[] = {2};
    CollectQueueElement* collectQueueElement2 = calloc(1, sizeof (CollectQueueElement));

    int signum = 0;

    FormatParams formatParams = {
        &collectQueue,
        &sdElementQueue,
        &signum
    };
    pthread_t identifier;

    values[0] = strdup("a");
    collectQueueElement->idsIDA = idsIDA;
    collectQueueElement->lineNum = 5;
    collectQueueElement->valuesLength = 1;
    collectQueueElement->lotNum = 6;
    collectQueueElement->values = values;
    collectQueueElement->time = 7;
    collectQueueElement->next = NULL;
    collectQueue.first = collectQueueElement;

    values2[0] = strdup("b");
    collectQueueElement2->idsIDA = idsIDA2;
    collectQueueElement2->lineNum = 12;
    collectQueueElement2->valuesLength = 1;
    collectQueueElement2->lotNum = 13;
    collectQueueElement2->values = values2;
    collectQueueElement2->time = 14;
    collectQueueElement2->next = NULL;
    collectQueueElement->next = collectQueueElement2;

    CU_ASSERT_FALSE(pthread_create(&identifier, NULL, format, (void*) &formatParams));
    sleep(1);
    signum = SIGTERM;
    sleep(1);
    /* Test arret du thread apres reception d'un signal */
    CU_ASSERT_EQUAL(pthread_cancel(identifier), ESRCH);

    /* Test formatage des deux elements */
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 7);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " lotNum=6 lineNum=5 1=\"a\"");
    CU_ASSERT_NOT_EQUAL(sdElementQueueElement->next, NULL);
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 14);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " lotNum=13 lineNum=12 2=\"b\"");
    CU_ASSERT_EQUAL(sdElementQueueElement->next, NULL);
}

void testPopCollectQueue()
{
    SDElementQueueElement* sdElementQueueElement = NULL;
    int result = 0;

    /* Creer une queue d'envoi vide */
    int probeID = 1;
    char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "127.0.0.1",
        "ea-probe",
        "abcd1234",
        &probeID,
        &transportMsgVersion,
        3,
        NULL
    };

    /* Creer une queue de collecte contenant deux elements */
    CollectQueue collectQueue = COLLECT_QUEUE_INITIALIZER;

    int valuesLength = 1;
    CollectQueueElement* collectQueueElement = calloc(1, sizeof (CollectQueueElement));
    gint idsIDA[] = {1};
    char** values = calloc(valuesLength, sizeof (char*));

    int valuesLength2 = 3;
    CollectQueueElement* collectQueueElement2 = calloc(1, sizeof (CollectQueueElement));
    gint idsIDA2[] = {2, 3, 4};
    char** values2 = calloc(valuesLength2, sizeof (char*));

    values[0] = strdup("a");
    collectQueueElement->idsIDA = idsIDA;
    collectQueueElement->lineNum = 9;
    collectQueueElement->valuesLength = valuesLength;
    collectQueueElement->lotNum = 10;
    collectQueueElement->values = values;
    collectQueueElement->time = 11;
    collectQueueElement->next = NULL;
    collectQueue.first = collectQueueElement;

    values2[0] = strdup("e");
    values2[1] = strdup("f");
    values2[2] = strdup("g");
    collectQueueElement2->idsIDA = idsIDA2;
    collectQueueElement2->lineNum = 6;
    collectQueueElement2->valuesLength = valuesLength2;
    collectQueueElement2->lotNum = 7;
    collectQueueElement2->values = values2;
    collectQueueElement2->time = 11;
    collectQueueElement2->next = NULL;
    collectQueueElement->next = collectQueueElement2;

    /* Test depile un element de la queue de collecte */
    result = popCollectQueue(&collectQueue, &sdElementQueue);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " lotNum=10 lineNum=9 1=\"a\"");
    CU_ASSERT_NOT_EQUAL(collectQueue.first, NULL);

    /* Test depile second element de la queue de collecte */
    result = popCollectQueue(&collectQueue, &sdElementQueue);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " lotNum=10 lineNum=9 1=\"a\"");
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " lotNum=7 lineNum=6 2=\"e\" 3=\"f\" 4=\"g\"");
    CU_ASSERT_EQUAL(collectQueue.first, NULL);

    /* Test depile alors que la queue de collecte est vide */
    result = popCollectQueue(&collectQueue, &sdElementQueue);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " lotNum=10 lineNum=9 1=\"a\"");
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " lotNum=7 lineNum=6 2=\"e\" 3=\"f\" 4=\"g\"");
    CU_ASSERT_EQUAL(collectQueue.first, NULL);

    free(sdElementQueue.first);
    free(sdElementQueueElement);
}

void testPushSDElementQueue()
{
    int probeID = 1;
    char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "127.0.0.1",
        "ea-probe",
        "abcd1234",
        &probeID,
        &transportMsgVersion,
        3,
        NULL
    };
    SDElementQueueElement* sdElementQueueElement = NULL;

    /* Test ajoute un element dans la queue*/
    int result = pushSDElementQueue(&sdElementQueue, "value=1", 5);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 5);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, "value=1");
    CU_ASSERT_EQUAL(sdElementQueueElement->next, NULL);

    /* Test ajoute un deuxieme element dans la queue*/
    result = pushSDElementQueue(&sdElementQueue, "value=2", 6);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 5);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, "value=1");
    CU_ASSERT_NOT_EQUAL(sdElementQueueElement->next, NULL);
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 6);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, "value=2");
    CU_ASSERT_EQUAL(sdElementQueueElement->next, NULL);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("formatcunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testFormat", testFormat)) ||
        (NULL == CU_add_test(pSuite, "testPopCollectQueue", testPopCollectQueue)) ||
        (NULL == CU_add_test(pSuite, "testPushSDElementQueue", testPushSDElementQueue)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

#ifndef NDEBUG
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
#else
    /* Run all tests using the CUnit Automated interface */
    CU_set_output_filename("cunit-result/format");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
