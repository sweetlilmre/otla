         
        org $8000


        ld hl,tablaCRTC
        xor a
buCRCT
        ld  b,&bc
        out (c),a     ; direcciona registro  (0, 1, ..., 17)

        ld  b,&bd
        ld  c,(hl)
        inc hl
        out (c),c     ; escribe registro

        inc a
        cp 18
        jr nz , buCRCT


        ld   bc,$7fff
        ld hl,tablaGA
        xor a

buGA    out  (c),a
        ld   d,(hl)
        inc  hl
        out  (c),d

        inc  a
        cp   16   ; 16 (tintas) 
        jr   nz , buGA         


        ld   d,(hl)
        inc  hl
        out  (c),d  ;  + multiregistroGA 

        ld   d,(hl)
     ;   inc  hl
        out  (c),d     ;+ tinta actual          
        
        pop  hl
        ld   sp,$fffd
        jp   (hl)  


                

tablaCRTC   defb 0,0,0,0  
            defb 0,0,0,0  
            defb 0,0,0,0  
            defb 0,0,0,0  
            defb 0,0 
                   ;CRTC: data register (0, 1, ..., 17)

        
tablaGA     defb 0,0,0,0  
            defb 0,0,0,0  
            defb 0,0,0,0  
            defb 0,0,0,0 
 
            defb 0,0 
 
                      ;ink value register (0, 1, ..., 15, 16)+Gate Array: multi configuration register
