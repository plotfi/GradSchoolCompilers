

icl main.cpp -Od -c
icl /Qopenmp -Od pseudorand_ap.cpp main.obj -o  pseudorand_ap.exe
icl /Qopenmp -Od blocksized_ap.cpp main.obj -o  blocksized_ap.exe
icl /Qopenmp -Od frontseq_ap.cpp   main.obj -o  frontseq_ap.exe
icl /Qopenmp -Od backseq_ap.cpp    main.obj -o  backseq_ap.exe
icl /Qopenmp -Od divconq_ap.cpp    main.obj -o  divconq_ap.exe
icl /Qopenmp -Od tiled_ap.cpp      main.obj -o  tiled_ap.exe
icl /Qopenmp -Od even_ap.cpp       main.obj -o  even_ap.exe
