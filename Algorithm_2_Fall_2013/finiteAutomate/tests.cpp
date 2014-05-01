#define BOOST_TEST_MODULE automataTest
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <memory>

#include "stringAutomate.h"

StringAutomateFactory factory;

typedef std::shared_ptr<IStringAutomate> _AutomatePtr;

#define MOVE(x, c) x = automaton->GetNextState(x, c); 

BOOST_AUTO_TEST_CASE(Test1) {
    _AutomatePtr automaton = factory.BuildAutomate("ananas");
    std::vector<int> result = automaton->ProcessText("anananas");
    BOOST_CHECK_EQUAL(result.size(), 1);
    if (result.size() == 1) {
        BOOST_CHECK_EQUAL(result[0], 2);
    }
}

BOOST_AUTO_TEST_CASE(Test2) {
    _AutomatePtr automaton = factory.BuildAutomate("word");
    std::vector<int> result = automaton->ProcessText("this sentence has so many words: word and word and word");
    BOOST_CHECK_EQUAL(result.size(), 4);
    if (result.size() == 4) {
        // Check all positions
        BOOST_CHECK_EQUAL(result[0], 26);
        BOOST_CHECK_EQUAL(result[1], 33);
        BOOST_CHECK_EQUAL(result[2], 42);
        BOOST_CHECK_EQUAL(result[3], 51);
    }
}

BOOST_AUTO_TEST_CASE(Test3) {
    _AutomatePtr automaton = factory.BuildAutomate("pattern");
    //std::vector<int> result = automaton->ProcessText("long text with \"pattern\" inside");
    IStringAutomate::State state = automaton->GetInitialState();
    IStringAutomate::State stNext = automaton->GetNextState(state, 'l');
    BOOST_CHECK_EQUAL(stNext.GetCommonPart(), 0);
    MOVE(stNext, 'p');
    BOOST_CHECK_EQUAL(stNext.GetCommonPart(), 1);
    MOVE(stNext, 'a');
    BOOST_CHECK_EQUAL(stNext.GetCommonPart(), 2);
    MOVE(stNext, 'o');
    BOOST_CHECK_EQUAL(stNext.GetCommonPart(), 0);
    BOOST_CHECK(stNext == state); 
    MOVE(stNext, 'p');
    BOOST_CHECK_EQUAL(stNext.GetCommonPart(), 1);
}

BOOST_AUTO_TEST_CASE(Test4) {
    _AutomatePtr automaton = factory.BuildAutomate("pattern");
    // std::vector<int> result = automataTest->ProcessText();
    IStringAutomate::State state = automaton->GetInitialState();
    MOVE(state, 'p');
    MOVE(state, 'a');
    BOOST_CHECK(automaton->IsFinal(state) == false);
    MOVE(state, 't');
    MOVE(state, 't');
    BOOST_CHECK(automaton->IsFinal(state) == false);
    MOVE(state, 'e');
    MOVE(state, 'r');
    MOVE(state, 'n');
    BOOST_CHECK(automaton->IsFinal(state) == true);
}

BOOST_AUTO_TEST_CASE(Test5) {
    _AutomatePtr automaton = factory.BuildAutomate("word and word");
    std::vector<int> result = automaton->ProcessText("text text word and word");
    BOOST_CHECK_EQUAL(result.size(), 1);
    if (result.size() == 1) {
        BOOST_CHECK_EQUAL(result[0], 10);
    }
}

BOOST_AUTO_TEST_CASE(Test6) {
    _AutomatePtr automaton = factory.BuildAutomate("ananas");
    IStringAutomate::State state = automaton->GetInitialState();
    // "anananas"
    MOVE(state, 'a');
    MOVE(state, 'n');
    MOVE(state, 'a');
    BOOST_CHECK_EQUAL(state.GetCommonPart(), 3);
    BOOST_CHECK(automaton->IsFinal(state) == false); 
    MOVE(state, 'n');
    MOVE(state, 'a');
    MOVE(state, 'n');
    BOOST_CHECK_EQUAL(state.GetCommonPart(), 4);
    MOVE(state, 'a');
    MOVE(state, 's');
    BOOST_CHECK(automaton->IsFinal(state) == true);
}

BOOST_AUTO_TEST_CASE(Test7) {
    _AutomatePtr automaton = factory.BuildAutomate("aaa");
    std::vector<int> result = automaton->ProcessText("aaaaaaaaaaaaaaaa");
    BOOST_CHECK_EQUAL(result.size(), 14);
    if (result.size() == 14) {
        for (int i = 0; i < 14; ++i) {
            BOOST_CHECK_EQUAL(result[i], i);
        }
    }
}

