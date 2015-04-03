1 cls:memory 30000:print"otla adjust..."
3 load"!":print"play test" 
7 d=&8000:call d:locate 1,1:d=d+256
8 print peek(d),peek(d+1)-128:print peek(d+2)/16,peek(d+5)+peek(d+4)/256:print peek(d+6)/16,peek(d+9)+peek(d+8)/256
9 goto 7