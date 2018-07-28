lexem.o: lexem.cpp lexem.h
parser.o: parser.cpp lexem.h parser.h
shell.o: shell.cpp lexem.h parser.h tree.h worker.h
tree.o: tree.cpp lexem.h tree.h parser.h
worker.o: worker.cpp lexem.h tree.h parser.h worker.h
