//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     Yelyzaveta Ovsiannikova <xlovsia00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Yelyzaveta Ovsiannikova
 * 
 * @brief Implementace testu hasovaci tabulky.
 */
#include <vector>
#include "gtest/gtest.h"
#include "white_box_code.h"
using namespace std;
//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//
class EmptyHashMap : public ::testing::Test
{
protected:
    hash_map* table = hash_map_ctor();
};

class NonEmptyHashMap : public ::testing::Test
{
protected:
    hash_map* table = hash_map_ctor();
    virtual void SetUp()
    {
        hash_map_put(table,"first",55);
        hash_map_put(table,"second",66);
        hash_map_put(table,"third",77);
    }
};

TEST_F(EmptyHashMap, hash_map_ctor)
{
    EXPECT_EQ(table->used, 0);
    EXPECT_EQ(table->allocated, 8);
    EXPECT_EQ(table->first, nullptr);
    EXPECT_NE(table, nullptr);
    EXPECT_EQ(table->last, nullptr);
}

TEST_F(EmptyHashMap, hash_map_dtor)
{
    hash_map_item_t* item = table->first;
    hash_map_dtor(table);
    EXPECT_EQ(table->allocated, 0);
    EXPECT_NE(item, table->first);
    EXPECT_EQ(table->used, 0);
}

TEST_F(EmptyHashMap, hash_map_put)
{
    EXPECT_EQ(hash_map_put(table,"first",55),OK);
    EXPECT_EQ(table->last->value, 55);
    EXPECT_EQ(table->used, 1);
    EXPECT_NE(table->first, nullptr);
    EXPECT_NE(table->last, nullptr);
    EXPECT_EQ(hash_map_put(table,"second",66),OK);
    EXPECT_EQ(table->last->value, 66);
    EXPECT_EQ(table->used, 2);
    EXPECT_STREQ(table->first->key,"first");
    EXPECT_EQ(table->first->value,55);
    EXPECT_EQ(hash_map_put(table,"first",55), KEY_ALREADY_EXISTS);
}

TEST_F(EmptyHashMap, hash_map_get)
{
        int value;
        EXPECT_EQ(hash_map_get(table, "aloha", &value), KEY_ERROR); //key not found
        hash_map_put(table,"first",55); //add new pair
        EXPECT_EQ(hash_map_get(table, "first", &value), OK); //key found
        EXPECT_EQ(value, 55); //value is correct
}

TEST_F(EmptyHashMap, hash_map_remove)
{
    hash_map_put(table,"first",55);
    EXPECT_EQ(hash_map_remove(table, "first"), OK);
    EXPECT_EQ(table->used, 1);
    EXPECT_EQ(hash_map_remove(table, "aloha"), KEY_ERROR);
}

TEST_F(EmptyHashMap, hash_map_size)
{
    EXPECT_EQ(hash_map_size(table), 0);
    hash_map_put(table,"first",55);
    EXPECT_EQ(hash_map_size(table), 1);
    hash_map_put(table,"second",66);
    EXPECT_EQ(hash_map_size(table), 2);
    hash_map_put(table,"third",77);
    EXPECT_EQ(hash_map_size(table), 3);

}

TEST_F(NonEmptyHashMap, hash_map_put)
{
    EXPECT_EQ(table->first->value, 55);
    EXPECT_EQ(table->last->value, 77);
    EXPECT_EQ(table->used, 3);
    EXPECT_EQ(hash_map_put(table, "third", 77), KEY_ALREADY_EXISTS);
    EXPECT_EQ(hash_map_put(table, "fourth", 88), OK);
    EXPECT_EQ(table->last->value, 88);
    EXPECT_EQ(table->used, 4);
    EXPECT_STREQ(table->first->key, "first");
}

TEST_F(NonEmptyHashMap, hash_map_get)
{
    int value;
    EXPECT_EQ(hash_map_get(table, "first", &value), OK);
    EXPECT_EQ(value, 55);
    EXPECT_EQ(hash_map_get(table, "second", &value), OK);
    EXPECT_EQ(value, 66);
    EXPECT_EQ(hash_map_get(table, "third", &value), OK);
    EXPECT_EQ(value, 77);
    EXPECT_EQ(hash_map_get(table, "aloha", &value), KEY_ERROR); //key not found
}

TEST_F(NonEmptyHashMap, hash_map_remove)
{
    EXPECT_EQ(hash_map_remove(table, "first"), OK);
    EXPECT_EQ(table->first->value, 66);
    EXPECT_EQ(table->last->value, 77);
    EXPECT_EQ( hash_map_remove(table, "second"), OK);
    EXPECT_EQ(table->first->value, 77);
    EXPECT_EQ(table->last->value, 77);
    EXPECT_EQ(hash_map_remove(table, "third"), OK);
    EXPECT_EQ(hash_map_remove(table, "aloha"), KEY_ERROR);
}

TEST_F(NonEmptyHashMap, hash_map_size)
{
    EXPECT_EQ(hash_map_size(table), 3);
    hash_map_put(table,"fourth",88);
    EXPECT_EQ(hash_map_size(table), 4);
    hash_map_remove(table,"first");
    EXPECT_EQ(hash_map_size(table), 4);
}

TEST_F(NonEmptyHashMap, hash_map_clear)
{
    hash_map_clear(table);
    EXPECT_EQ(table->used, 0);
    EXPECT_EQ(table->allocated, 8);
}

TEST_F(NonEmptyHashMap, hash_map_capacity)
{
    EXPECT_EQ(hash_map_capacity(table), 8);
    hash_map_put(table,"fourth",88);
    EXPECT_EQ(hash_map_capacity(table), 8);
    hash_map_put(table,"fifth",99);
    EXPECT_EQ(hash_map_capacity(table), 8);
    hash_map_put(table,"sixth",100);
    EXPECT_EQ(hash_map_capacity(table), 16);
}

TEST_F(NonEmptyHashMap, hash_map_pop)
{
    int value;
    hash_map_get(table, "aloha", &value);
    EXPECT_EQ(hash_map_pop(table, "aloha", &value), KEY_ERROR);
    EXPECT_EQ(hash_map_pop(table, "second", &value), OK);
    EXPECT_EQ(value, 66);
    EXPECT_FALSE(hash_map_contains(table, "second"));
    EXPECT_EQ(hash_map_capacity(table), 8);
    EXPECT_EQ(hash_map_size(table), 3);
    EXPECT_EQ(hash_map_pop(table, "first", &value), OK);
    EXPECT_EQ(value, 55);
    EXPECT_EQ(hash_map_capacity(table), 8);
    EXPECT_EQ(hash_map_size(table), 3);
    EXPECT_FALSE(hash_map_contains(table, "first"));
    EXPECT_EQ(hash_map_pop(table, "third", &value), OK);
    EXPECT_EQ(value, 77);
    EXPECT_EQ(hash_map_capacity(table), 8);
    EXPECT_EQ(hash_map_size(table), 3);
    EXPECT_FALSE(hash_map_contains(table, "third"));
}

TEST_F(NonEmptyHashMap, hash_map_contains)
{
    EXPECT_TRUE(hash_map_contains(table, "first"));
    EXPECT_TRUE(hash_map_contains(table, "second"));
    EXPECT_TRUE(hash_map_contains(table, "third"));
    EXPECT_FALSE(hash_map_contains(table, "aloha"));
}

TEST_F(NonEmptyHashMap, hash_map_reserve)
{
    EXPECT_EQ(hash_map_reserve(table, 8), OK);
    EXPECT_EQ(hash_map_reserve(table, 16), OK);
    EXPECT_EQ(hash_map_capacity(table), 16);
    EXPECT_EQ(hash_map_size(table), 3);
    EXPECT_EQ(hash_map_reserve(table, 2), VALUE_ERROR);
}
/*** Konec souboru white_box_tests.cpp ***/
