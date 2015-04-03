 
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

start_loading

         
        ld    a,$0f    ;xor    a
        out   ($aa),a  ; motor cassette on y caps on

       
        




velozloader



        di

        ld    a,14
        out  ($a0),a

        ld  bc, $f2a2 

pii_0   ld  de, $0810

pii     
        ld  (mem1),a 
        ld a,$87

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

SYNC    cp  $39+ $87  ;  47 +$87   
        rl  e
        cp  $29+ $87  ;  34 +$87   
        jr  nc, pii

        inc e
        jr  nz , pii_0

syn_LOW in  f,(c)
        jp  m, syn_LOW

        cp  $19 + $87     


fin

        ld     (mem1+1),a
        rlca
        rlca
        rlca
        rlca

        call print_hex

        ld     a,(mem1+1)

        call print_hex


        ld     a,(mem1)
        rlca
        rlca
        rlca
        rlca

        call print_hex

        ld     a,(mem1)

        call print_hex


   

        ;ld    a,$7f    ;xor    a
        ;out   ($aa),a  ; motor cassette off y caps on

        ld    bc,$7faa
        out   (c),b

	ei
	ret

mem1   defb  0, 0


         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;





         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
     


print_hex
        and    $0f
        or     $30
        cp     $3a

        jr     c , menor0
        add    a,$41-$3a

menor0  out    ($98),a

        ret

;final   nop
