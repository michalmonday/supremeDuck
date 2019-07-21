#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef struct ProtocolFunc{
  char * begining;
  bool has_ending;
  void (*func)(char *);

  ProtocolFunc(char *b, bool he, void(*f)()) : begining(b), has_ending(he), func(f){}
  ProtocolFunc(char *b, void(*f)()) : begining(b), has_ending(false), func(f){}
};

namespace Protocol {
  extern void Check(char *str);
  extern int protocol_size;
}





















#endif
