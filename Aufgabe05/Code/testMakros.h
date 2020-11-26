#ifndef __TEST_MAKROS
#define __TEST_MAKROS

// Makro, der einen Test aufruft. Falls der Test scheitert, wird
// der Name der durchgeführten Testfunktion in roter Farbe auf dem Bildschirm 
// ausgegeben und die Variable result aus false gesetzt.
#define PERFORM_AND_OUTPUT(test)      \
{if (!test())                         \
   {                                  \
   std::cout << " ---- Gescheitert ";  \
   printScreenColorOnceVal(std::cout, RED_SCREEN_COLOR, #test); \
   std::cout << " ----\n";                 \
   result = false;                    \
}else                                 \
{                                     \
   std::cout << " ---- Erfolgreich ";  \
   printScreenColorOnceVal(std::cout, GREEN_SCREEN_COLOR, #test); \
   std::cout << " ----\n";                 \
   result = true;                    \
}}

#endif

