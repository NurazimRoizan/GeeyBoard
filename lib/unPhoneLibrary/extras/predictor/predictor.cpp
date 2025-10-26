// predictor.cpp

#include <iostream>
#include <cstdint>
#include "predictor.h"

/* generate word set predictions for numeric input symbols;
   see main for usage; example words.h types:
static const uint16_t NUM_WORDS = 2000;
static const char *words[] = {  // in order of frequency
  "the",               //    0
  "to",                //    2
...
static const uint16_t NUM_SUGGS = 2446;
static const uint16_t suggestionSets[][NUM_SUGGS] = {
//arity wordnum(s)...                     index: symseq(s)...
  {  1,  1157, },                       //    0: 224 2245 22454 224548 2245489
  {  1,  333, },                        //    1: 2253
...
static const uint16_t NUM_STATES = 4746;
static const int16_t states[][NUM_STATES] = {
// sugset numds desc(s)[sym/state]....                             ... // index
  {    -1,  8,  2,    1,  3, 1081,  4, 1835,  5, 2311,  6, 2550, },    //    0
*/
class Predictor {
  static const uint16_t MAX_WORD_LEN = 20; // max characters in a word
  uint16_t STATE_SUGGESTIONS = 0; // position of sugset num in state array
  uint16_t STATE_NUM_DESCS = 1;   // position of sugset num in state array
  uint16_t STATE_1ST_DESC = 2;    // position of 1st descendant in state array

  uint16_t state = 0;         // array index of current state (0 is root)
  char history[MAX_WORD_LEN]; // symbols consumed so far
  uint16_t histlen = 0;       // number of symbols in history
  uint16_t sugiter = 1;       // array index for suggestion set iterator
public:
  Predictor() {
  }
  void print() {
    printf("predictor: state(%d) histlen(%d) sugiter(%d) descendants: ",
      state, histlen, sugiter);
    uint16_t numDescInts = states[state][STATE_NUM_DESCS] * 2;
    for(uint16_t i = 0; i < numDescInts; i += 1) {
      const char *bracket = "|";
      if(i % 2) bracket = " ";
      printf("%s%d%s", bracket, states[state][STATE_1ST_DESC + i], bracket);
    }
    printf("\n");
  }
  void reset() {
    histlen = 0;
    state = 0;
    sugiter = 1;
  }
  const int16_t suggest(uint8_t symbolSeen) { // rtn sugset num or -1
    const int16_t *sp = states[state];
    uint16_t numDescendantInts = sp[STATE_NUM_DESCS] * 2;
    printf("symbolSeen(%d) numDescInts(%d)\n", symbolSeen, numDescendantInts);
    for(uint16_t i = 0; i < numDescendantInts; i += 2) {
      uint16_t consumedSymbol =  sp[STATE_1ST_DESC + i + 0];
      uint16_t descendantState = sp[STATE_1ST_DESC + i + 1];
      printf("consumedSymbol(%d) descendantState(%d) symbolSeen(%d)\n",
        consumedSymbol, descendantState, symbolSeen);
      if(symbolSeen == consumedSymbol) {
        history[histlen++] = symbolSeen;
        state = descendantState;
        return states[state][STATE_SUGGESTIONS];
      }
    }
    return -1;
  }
  const char *next() { // pointer to a current suggestion word, or NULL
    const uint16_t *sugset =
      suggestionSets[states[state][STATE_SUGGESTIONS]];
    uint16_t numSuggs = sugset[0];
    uint16_t nextWord;
    if(sugiter <= numSuggs) {
      nextWord = sugset[sugiter++];
      return words[nextWord];
    } else 
      sugiter = 1;
    return NULL;
  }
  const char *first() { // pointer to first suggestion, or NULL
    const uint16_t *sugset =
      suggestionSets[states[state][STATE_SUGGESTIONS]];
    if(sugset[0] > 0)          // there's at least 1 suggestion
      return words[sugset[1]]; // return it
    return NULL;               // there were no suggestions (at root?)
  }
  const int16_t getState() { return state; }
};

#ifdef  PREDICTOR_MAIN // for testing
int main() {
  printf("using lexicon of %d words\n", (int) NUM_WORDS);
  Predictor p;                                    // *** usage example ***

  setbuf(stdout, NULL);
  setbuf(stdin, NULL);

  char c; 
  while(true) {
    p.print();                                    // *** usage example ***
    printf("input a symbol:\n");
    c = getchar();
    if(c == '\n') {
      printf("no input\n");
      continue;
    } else if(c == 'q') { // quit
      printf("quitting\n");
      exit(0);
    } else {
      getchar();
    }

    const char *selection = NULL;
    switch(c) {
      case ' ': // space or...
      case '0': // ...zero: select first candidate
        selection = p.first();                    // *** usage example ***
        if(selection)
          printf("selecting word %s\n", selection);
        else
          printf(
            "no word to select from state %d\n",
            p.getState()                          // *** usage example ***
          );
        p.reset();                                // *** usage example ***
        break;
      default:  // 1-9
        if(p.suggest(c - '0') >= 0) {             // *** usage example ***
          printf("suggestions: ");
          const char *suggested;
          while( (suggested = p.next()) != NULL ) // *** usage example ***
            printf("%s ", suggested);
          printf("\n");
        }
    }

    printf("\n");
  }

  return 0;
}
#endif
