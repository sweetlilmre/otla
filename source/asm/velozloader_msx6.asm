 
dir_HAIGH        equ     &f4

        org  dir_HAIGH*&100 +  &30 -14      
        
           
        ld    hl, $c030
        ld    de, table
        ld    bc, 280-14
        ldir  
        jp    start_loading



        org  dir_HAIGH*$100 + $30
        

        
table
       defb $4c,$4c
       defb $4c,$4c
       defb $4c,$4c,$4c,$4c,$4c
       defb $4d,$4d,$4d,$4d,$4d
       defb $4e,$4e,$4e,$4e,$4e
       defb $4f,$4f,$4f,$4f,$4f
       defb $4f,$4f,$4f


strerror   
         defb 32,'l','o','a','d','i','n','g'
         defb 32,'e','r','r','o','r' 




         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


velozloader

        ld    d,(hl)
        dec   hl
        ld    e, (hl)
        push  de       
        push  hl


        di

        ld    a,14
        out  ($a0),a

        ld  bc, $f2a2 

pii_0   ld  de, $0810

pii     ld a,$87

        inc hl ;
        dec hl ;     ;out ($99),a

pi_LOW  inc a
        in  f,(c)
        jp  m, pi_LOW

        inc hl
        dec hl       ;out ($99),a 

pi_HIGH inc a
        in  f,(c)
        jp  p, pi_HIGH

SYNC    cp  $33+ $87  ;  47 +$87   
        rl  e
        cp  $25+ $87  ;  34 +$87   
        jr  nc, pii

        inc e
        jr  nz , pii_0

syn_LOW in  f,(c)
        jp  m, syn_LOW

        cp  $18 + $87    ;  17 +$87  
        ld  a, b ; e= $f2  

        jr  c, no_inv

        xor d ; d=8

syn_hi  in  f,(c)
        jp  p, syn_hi
no_inv
        ld  (p1),a    ;,a
        xor d
        ld  (m1),a    ;,a

        
        ex  de,hl
        dec de    
        xor a
        ld  b,a
        jr  enter

         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


full    and  H
        ld   (de),a
        xor  b
        ld   b,a
        ld   a, $fe   ;  b=$fe    ; 10 + 26   + (6) = 42
enter
more
HIGH    inc  l
        in  f , (c)
p1      jp  p , HIGH

        inc  de

        ld  h, dir_HAIGH  ;&80
        ld  h,(hl)

        ld  l, table - (dir_HAIGH  * &100) -2;

        rlca
        rla            ;35  + (6)   = 41

LOW     inc  l
        in   f,(c)
m1      jp   m, LOW    ; 28/26+(3)

        jp  nC, full

        dec  de
        ld   b,b

        and  H
        jp   m, more         ;10 + 24  + (5) = 39

                       ;39+42+ (29)*2 = 139

         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

fin
       
        pop  hl
        pop  de
        ld   (hl),e
        inc  hl
        ld   (hl),d

        ld   a,b
        or   a

        ei  
        ret z   ;ret z

        ld hl,strerror
        ld bc,$1398
        otir   
        pop hl
        pop hl
        ret
 
         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
       
 
       defb   '0' 
header defw   $c200
bu     ret 
       defw   dexec    ; cd/ld de,/c9 

entra_mbloq       
         
        ld    hl,header -1
        inc  (hl)
        ld   a,(hl)
        out  ($98),a

        call  velozloader

        ld    hl,(header)
        call  velozloader        
        jr    bu 

               ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

start_loading
        ld    hl, dexec +12 ; pila

        ld    a,170
        ld    ($ffff),a
         
        ld    a,$0f    ;xor    a
        out   ($aa),a  ; motor cassette on y caps on

        call  entra_mbloq
        
        ;ld    a,$7f    ;xor    a
        ;out   ($aa),a  ; motor cassette off y caps on

        ld    bc,$7faa
        out   (c),b

        ld    hl ,(bu+1)
        jp    (hl)

dexec
       ; ret

         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

print_test
        ld     hl,$c200       ; hldesde ini-1
        ld     (hl),a         ; a=res chk

        xor    a              ;ld     a,1
        out    ($99),a        ; Send LOW byteto VDP
                              ;ld      a,h
        ld     a, 0x80        ;or      01000000b; Set bit 6 in High byte to
                              ; indicate WRITE
        out    ($99),a        ; Send MSB to VDP

        ld     bc, $02fe

loop    ld     a,(hl)
        rlca
        rlca
        rlca
        rlca

        call print_hex

        ld     a,(hl)

        call print_hex

        ld     (hl),0x11 

        inc    hl
        dec    bc
        inc    b
        djnz   loop
        
        ret

print_hex
        and    $0f
        or     $30
        cp     $3a

        jr     c , menor0
        add    a,$41-$3a

menor0  out    ($98),a

        ret

;final   nop
