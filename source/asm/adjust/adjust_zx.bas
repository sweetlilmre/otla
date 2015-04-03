1 clear 49663
2 load "" code
3 cls: print at 11,5;flash 1;"PLAY TEST SIGNAL WAV"


5 let d=49664 
6 randomize usr d

9 cls

10 let d=d +256

11 print peek (d) , peek (d+1)-128,peek (d+2)/16, peek (d+5)+peek (d+4)/256,peek (d+6)/16, peek (d+9)+peek (d+8)/256,peek (d-1), peek (d-2)


20 goto 5

29 let d=49950-1
30 gosub 100

59 let d=d+1
60 gosub 100

90 goto 5

100 let x=0: let y=0
110 for i=1 to 64 step 2
120 let x=x+peek(d+i)
130 next i
140 let x=INT (x/32+0.5): print x/16,
150 for i=1 to 64 step 2
160 let y=y+(peek(d+i)-x)*(peek(d+i)-x)
170 next i
180 print  y/256;"-*-"
200 return