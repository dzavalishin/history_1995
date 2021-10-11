#include <stdio.h>

#include "cp1251.c"
#include "koi8.c"

char rbyte(
           unsigned short c,
           unsigned short *tf,	/* Source encoding table (language) */
           unsigned short *tt	/* Dest. encoding table (language) */
          )
    {
    register short i;

    if(c < 128)
        return c;

    c = tf[c-128];
    
    for(i=0;i<128;i++)
        if(c == tt[i])
            return i+128;

    return ' ';
    }




void mktab(
      unsigned short *tf,	// Source encoding table (language)
      unsigned short *tt	// Dest. encoding table (language)
     )
    {
    for( int i = 128; i < 256; i++ )
        {
        if( !(i % 0x8) ) printf("\n");
        printf( "0x%02x, ", rbyte( i, tf, tt ) );
        }
    }

void main( void )
    {

    printf( "unsigned char cp1251_to_koi8 [128] =\n{\n" );
    mktab( cp1251, koi8 );
    printf( "\n};\n" );

    printf( "\n\nunsigned char koi8_to_cp1251 [128] =\n{\n" );
    mktab( koi8, cp1251 );
    printf( "\n};\n" );
    
    }



