//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Yelyzaveta Ovsiannikova  <xovsia00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Yelyzaveta Ovsiannikova
 * @brief Implementace testu binarniho stromu.
 */
#include <vector>
#include "gtest/gtest.h"
#include "red_black_tree.h"
using namespace std;
//============================================================================//
// ** ZDE DOPLNTE TESTY **
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
    protected:
    BinaryTree tree;
    virtual void SetUp()
    {
        int nodes[] = {10,85,15,70,20,60,30,50,65,80,90,40,5,55};
        for (int value: nodes)
            tree.InsertNode(value);
    }
};

class TreeAxioms : public NonEmptyTree{
};

TEST_F(EmptyTree, InsertNode)
{
    //inserting a new key into an empty tree
    auto result1 = tree.InsertNode(2);
    EXPECT_TRUE(result1.first);
    EXPECT_EQ(result1.second->key, 2);
    //the key already exists in the tree
    auto result2 = tree.InsertNode(2);
    EXPECT_FALSE(result2.first);
    EXPECT_EQ(result2.second, result1.second);
}

TEST_F(EmptyTree, DeleteNode)
{
    //deleting node from an empty tree
    EXPECT_FALSE(tree.DeleteNode(2));
    //deleting a node that does not exist in the tree
    tree.InsertNode(10);
    EXPECT_FALSE(tree.DeleteNode(21));
    EXPECT_TRUE(tree.FindNode(10) != nullptr);
    EXPECT_TRUE(tree.FindNode(21) == nullptr);
    EXPECT_TRUE(tree.DeleteNode(10));
}

TEST_F(EmptyTree, FindNode)
{
    //searching for a node in an empty tree
    EXPECT_FALSE(tree.FindNode(1));
    EXPECT_EQ(nullptr, tree.FindNode(1));
    //searching for a node which was inserted into the tree
    tree.InsertNode(5);
    auto node = tree.FindNode(5);
    EXPECT_TRUE(node != nullptr);
    EXPECT_EQ(node->key, 5);
    //searching for nonexisting node
    EXPECT_FALSE(tree.FindNode(1));
    EXPECT_EQ(nullptr, tree.FindNode(1));
}

TEST_F(NonEmptyTree, InsertNode)
{
    //inserting the node that already exists in the tree
    auto existingPair = tree.InsertNode(10);
    auto existingNode = existingPair.second;
    EXPECT_NE(existingNode, nullptr);
    EXPECT_FALSE(existingPair.first);
    EXPECT_EQ(existingNode->key, 10);
    //inserting a new node into the tree,checking if previous nodes still exist in the tree
    auto newPair = tree.InsertNode(27);
    auto newNode = newPair.second;
    EXPECT_NE(newNode, nullptr);
    EXPECT_TRUE(newPair.first);
    EXPECT_EQ(newNode->key, 27);
    EXPECT_TRUE(tree.FindNode(10) != nullptr);
}

TEST_F(NonEmptyTree, DeleteNode)
{
    //deleting a node that exists in the tree
    EXPECT_TRUE(tree.DeleteNode(10));
    EXPECT_FALSE(tree.FindNode(10));
    //deleting a node that does not exist in the tree
    EXPECT_FALSE(tree.DeleteNode(2));
    EXPECT_EQ(nullptr, tree.FindNode(2));
}

TEST_F(NonEmptyTree, FindNode)
{
    //searching for a node that exists in the tree
    auto node = tree.FindNode(10);
    EXPECT_TRUE(node != nullptr);
    EXPECT_EQ(node->key, 10);
    //searching for a node that does not exist in the tree
    auto node1 = tree.FindNode(100);
    EXPECT_TRUE(node1 == nullptr);
    EXPECT_EQ(node1, nullptr);
}

TEST_F(TreeAxioms, Axiom1)
{
    //checking if all leaf nodes are black
    vector<BinaryTree::Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);
    for (auto node : leafNodes) {
        EXPECT_EQ(node->color, BinaryTree::BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2)
{
    //checking if all red nodes have black children
    vector<BinaryTree::Node_t *> nodes;
    tree.GetAllNodes(nodes);
    for (auto node : nodes)
    {
        if (node->color == BinaryTree::RED)
        {
            EXPECT_EQ(node->pLeft->color, BinaryTree::BLACK);
            EXPECT_EQ(node->pRight->color, BinaryTree::BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3)
{
    //checking if all paths from leaf nodes to root have the same number of black nodes
    vector<BinaryTree::Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);
    auto blackNodes = 0;
    auto node = leafNodes[0];
    while (node != nullptr)
    {
        if (node->color == BinaryTree::BLACK)
        {
            blackNodes++;
        }
        node = node->pParent;
    }
    for (auto leafNode : leafNodes)
    {
        auto blackNodes1 = 0;
        auto node1 = leafNode;
        while (node1 != nullptr)
        {
            if (node1->color == BinaryTree::BLACK)
            {
                blackNodes1++;
            }
            node1 = node1->pParent;
        }
        EXPECT_EQ(blackNodes, blackNodes1);
    }
}
/*** Konec souboru black_box_tests.cpp ***/

