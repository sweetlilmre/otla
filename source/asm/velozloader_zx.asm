


       
        org  $ff58 -13

        ld  hl, 23794
        ld  de, table
        ld  b,1
        ldir
        jp  start_loading


        org  $ff58
           
table
       defb $4c
       defb $4c,$4c,$4c,$4c,$4c
       defb $4d,$4d,$4d,$4d,$4d
       defb $4e,$4e,$4e,$4e,$4e
       defb $4f,$4f,$4f,$4f,$4f
       defb $4f,$4f


velozloader  


        ld    d, (hl)
        dec   hl
        ld    e, (hl)
        push  de

        ld    e , $08

        ld    bc, $eafe

        push  hl

        di 

pii_0   ld   d,e

pii     xor   a
        out   (c),e

pi_LOW  inc   a
        in    f,(c)
        jp    po, pi_LOW

        out   (c),b

pi_HIGH inc   a
        in    f,(c)
        jp    pe, pi_HIGH

SYNC    cp    $39
        rl    d
        cp    $29
        jr    nc, pii

        inc   d
        jr    nz , pii_0

syn_LOW in    f,(c)
        jp    po, syn_LOW

        cp    $19
        ld    a,b          ; b= $ea

        jr    c, no_inv

        xor   e            ; e=8

syn_hi  in    f,(c)
        jp    pe, syn_hi

no_inv
        ld   (pe1),a
        xor  e            ; e=8
        ld   (po1),a

        ex   de,hl

        xor  a
        ld   ixl,a

        jr  enter

full    and  H
        ld   (de),a
        inc  de
        xor  ixl
        ld   ixl,a
        ld   a, c;      ; 10 +37 = 47

enter
more
HIGH    inc  hl
        in  f , (c)
pe1     jp  pe , HIGH

        ld h,$ff
        ld h,(hl)

        ld l,(table-$ff00)- 5

        rlca
        rla                     ; 29

LOW     inc  hl
        in  f,(c)
po1     jp  po, LOW      

        jp  nC, full

        dec  b
        set  3,b
        out (c),b
        and  H
        jp  m, more   ; 10 + 24 +4+ 10 = 48
    
        pop  hl
        pop  bc
        ld   (hl),c
        inc  hl
        ld   (hl),b

        ld   a,ixl
        or   a

dexec        
        ei
        ret   z      
        rst   8
        defb  26

header  defw  $3fff
bu      jp    dexec ;  / c3 / c9 / cd

         
        ld    hl,header -1
        call  velozloader

        ld    hl,(header)
        call  velozloader        
        jr    bu 

start_loading        

        ld   de,00
        jr   bu +3

