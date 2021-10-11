#include	<stdio.h>
#include	<string.h>
#include	"Ftn_Str.h"
#include	"fido_pkt.h"

char	*prod_name( uint code );
char	*print_capa( uint cw );

void	usage();

int main( int ac, char **av ) 
    {
    FILE		*fp;
    fido_pkt_hdr	h;
    fido_pmsg_hdr	m;
    short      		leader;
    bool		twoplus = No;			// 2+ packet?

    if( ac != 2 )
        {
        usage();
        return 1;
        }


    fp = fopen( av[1], "rb" );
    if( fp == NULL )
        {
        printf("Can't open %s\n", av[1] );
        return 1;
        }

    fread( &h, sizeof(h), 1, fp );

    if(
       ((h.CapWord & 0xFFu) == (h.CapValid >> 8 & 0xFFu)) &&
       ((h.CapWord >> 8 & 0x7Fu) == (h.CapValid & 0x7Fu)) &&
       h.CapWord != 0
      )
        twoplus = Yes;

//	printf("Pos1: %ld\n", ftell( fp ));

    printf("From: %d:%d:%d/%d.%d\n", h.origZone, h._origZone, h.origNet, h.origNode, h.origPoint );
    printf("To  : %d:%d:%d/%d.%d\n", h.destZone, h._destZone, h.destNet, h.destNode, h.destPoint );
    printf("Type: %d%s\n", h.pkt_type, twoplus ? "+" : "" );
    printf("CapW: %s\n", print_capa( h.CapWord ) );
    printf("Code: 0x%x.%x (%s)\n", h.hi_product_code, h.lo_product_code,
           prod_name( (((uint)h.hi_product_code)<<8) | (((uint)h.lo_product_code) ) & 0xFFu) );
    printf("Vers: %d.%d\n", h.hi_version_no, h.lo_version_no );
    printf("Baud: %d\n", h.baud );

    fread( &leader, 2, 1, fp );
    //	printf("Pos2: %ld\n", ftell( fp ));

    printf("\nLeader: %d\n", leader );

    fread( &m, sizeof(m), 1, fp );
    //	printf("Pos3: %ld\n", ftell( fp ));

    printf("From: %d/%d\n", m.origNet, m.origNode );
    printf("To  : %d/%d\n", m.destNet, m.destNode );
    printf("Cost: %d\n", m.cost );
    //	printf("Attr: %d\n", m.attr );
    //	printf("Date: %.20s\n", m.date );


    return 0;
    }




char    *
print_capa( uint cw )
    {
    static char    buf[200];
    int            i;

    buf[0] = '\0';

    if( cw & 0x80u )
        strcat( buf, "RFC822 " );

    for( i = 2; i < 16; i++, cw >>= 1 )
        {
        if( cw && 1u )
            {
            char    b[5];
            sprintf( b, "%d ", i );
            strcat( buf, b );
            }
        }

    return buf;
    };


void
usage()
	{
	printf(
        "Packet file dumper, v1.00, Copyright (C) 1992 by Infinity Soft\n"
        "Written by dz when he had nothing else to do :)\n"
		"\n"
		"Displays FIDO .PKT file header.\n"
		"\n"
		"Usage: pdump <pkt_file_name>\n"
		);
	}


struct pname
	{
	int	code;
	char	*name;
	};

struct pname	ptab[] = 
	{
    0xCC,   "UU2 Gate, MS-DOS, Packer, Dmitry Zavalishin, 2:5020/23.77",
    0x00,   "Fido, MS-DOS, Packer/mailer, Tom Jennings, 1:125/111",
    0x01,   "Rover, MS-DOS, Packer/mailer, Bob Hartman, 1:104/501",
    0x02,   "SEAdog, MS-DOS, Packer/mailer, Thom Henderson, 1:107/542.1",
    0x04,   "Slick-150, HP-150, Packer/mailer, Jerry Bain, 1:170/220",
    0x05,   "Opus, MS-DOS, Packer/mailer, Doug Boone, 1:119/5",
    0x06,   "Dutchie, MS-DOS, Packer/mailer, Henk Wevers, 2:500/1",
    0x08,   "Tabby, Macintosh, Packer/mailer, Michael Connick, 1:107/412",
    0x0A,   "Wolf-68k, CPM-68k, Packer/mailer, Robert Heller, 1:321/153",
    0x0B,   "QMM, QNX, Packer/mailer, Rick Duff, 1:167/201",
    0x0C,   "FrontDoor, MS-DOS, Packer/mailer, Joaquim Homrighausen, 2:270/17",
    0x11,   "MailMan, MS-DOS, Packer, Ron Bemis, 1:124/1113",
    0x12,   "OOPS, MS-DOS, Packer, Tom Kashuba, 1:167/1",
    0x13,   "GS-Point, Atari ST, Packer/mailer, Harry Lee, 1:321/202",
    0x14,   "BGMail, ????, ????, Ray Gwinn, 1:265/1",
    0x19,   "BinkScan, MS-DOS, Packer, Shawn Stoddard, 1:362/101",
    0x1A,   "D'Bridge, MS-DOS, Packer/mailer, Chris Irwin, 1:18/68",
    0x1B,   "BinkleyTerm, MS-DOS, Mailer, Vince Perriello, 1:141/491",
    0x1C,   "Yankee, MS-DOS, Packer, Randy Edwards, 1:132/555",
    0x1E,   "Daisy, Apple ][, Packer/mailer, Raymond & Ken Lo, 3:700/1",
    0x1F,   "Polar Bear, ????, Packer/mailer, Kenneth McLeod, 1:101/190",
    0x20,   "The-Box, MS-DOS/Atari ST, Packer/mailer, Jac Kersing & Arjen Lentz, 2:283/333 & 2:283/512",
    0x21,   "STARgate/2, OS/2, Packer/mailer, Shawn Stoddard, 1:362/101",
    0x22,   "TMail, MS-DOS, Packer, Larry Lewis, 3:713/600.1701",
    0x23,   "TCOMMail, MS-DOS, Packer/mailer, Mike Ratledge, 1:372/888",
    0x24,   "Bananna, Macintosh, Packer/mailer, Joe Keenan, 1:109/401",
    0x25,   "RBBSMail, MS-DOS, Packer, Jan Terpstra, 2:512/10",
    0x26,   "Apple-Netmail, Apple ][, Packer/mailer, Bill Fenner, 1:129/87",
    0x27,   "Chameleon, Amiga, Mailer, Juergen Hermann, 2:241/2.12",
    0x28,   "Majik Board, MS-DOS, Packer/mailer, Dale Barnes, 1:375/11",
    0x29,   "QMail, MS-DOS, Packer, Greg Dawson, 1:106/111",
    0x2A,   "Point And Click, Amiga, Packer, Rob Tillotson, 1:201/40.302",
    0x2B,   "Aurora Three Bundler, MS-DOS, Packer, Oliver McDonald, ????",
    0x2C,   "FourDog, MS-DOS, Packer, Shay Walters, 1:376/12",
    0x2D,   "MSG-PACK, MS-DOS, Packer, Tom Hendricks, 1:261/662",
    0x2E,   "AMAX, MS-DOS, Packer, Alan Applegate, 1:104/36",
    0x2F,   "Domain Communication System, ????, ????, Hal Duprie, 1:101/106",
    0x30,   "LesRobot, ????, Packer, Lennart Svensonn, 2:501/2",
    0x31,   "Rose, MS-DOS, Packer/mailer, Glen Jackson, 1:100/617",
    0x32,   "Paragon, Amiga, Packer/mailer, Jon Radoff, 1:322/545",
    0x33,   "BinkleyTerm/oMMM/ST, Atari ST, Packer/mailer, Peter Glasmacher, 1:143/9",
    0x34,   "StarNet, Atari ST, Mailer, Eric Drewry, 1:322/566",
    0x35,   "ZzyZx, MS-DOS, Packer, Claude Warren, 1:104/62",
    0x36,   "QEcho, MS-DOS, Packer, The QuickBBS Group, 1:363/1701",
    0x37,   "BOOM, MS-DOS, Packer, Andrew Farmer, 1:163/115",
    0x38,   "PBBS, Amiga, Packer/mailer, Todd Kover, 1:261/1028",
    0x39,   "TrapDoor, Amiga, Mailer, Maximilian Hantsch, 2:310/6",
    0x3A,   "Welmat, Amiga, Mailer, Michael Richardson, 1:163/109.10",
    0x3B,   "NetGate, Unix-386, Packer, David Nugent, 3:632/348",
    0x3C,   "Odie, MS-DOS, Mailer, Matt Farrenkopf, 1:105/376",
    0x3D,   "Quick Gimme, CPM-80/MS-DOS, Packer/mailer, Laeeth Isaacs, 2:254/18",
    0x3E,   "dbLink, MS-DOS, Packer/mailer, Chris Irwin, 1:18/68",
    0x3F,   "TosScan, MS-DOS, Packer, Joaquim Homrighausen, 2:270/17",
    0x40,   "Beagle, MS-DOS, Mailer, Alexander Holy, 2:310/11.32",
    0x41,   "Igor, MS-DOS, Mailer, Harry Lee, 1:321/202",
    0x42,   "TIMS, MS-DOS, Packer/mailer, Bit Bucket Software, 1:104/501",
    0x43,   "Isis, MS-DOS, Packer/mailer, Mike Bryeans, ????",
    0x44,   "AirMail, MS-DOS, Packer/mailer, Joaquim Homrighausen, 2:270/17",
    0x45,   "XRS, MS-DOS, Packer, Mike Ratledge, 1:372/888",
    0x46,   "Juliet Mail System, Amiga, Packer, Gregory Kritsch, 1:163/109.30",
    0x47,   "Jabberwocky, Macintosh, Packer, Eric Larson, 1:260/330",
    0x48,   "XST, MS-DOS, Packer, Wayne Michaels, 1:380/100",
    0x49,   "MailStorm, Amiga, Packer, Russel Miranda, 1:268/106",
    0x4A,   "BIX-Mail, ????, Mailer, Bob Hartman, 1:104/501",
    0x4B,   "IMAIL, MS-DOS, Packer, Fabiano Fabris, 2:310/11.22",
    0x4C,   "FTNGate, MS-DOS, Packer, Jason Steck, 1:104/424",
    0x4D,   "RealMail, MS-DOS, Packer, Taine Gilliam, 1:372/42",
    0x4E,   "Lora-CBIS, MS-DOS, Mailer, Marco Maccaferri, 2:332/16",
    0x4F,   "TDCS, PDP-11, Packer/mailer, Terry Ebdon, 2:254/6",
    0x50,   "InterMail, MS-DOS, Packer/mailer, Peter Stewart, 1:135/21",
    0x51,   "RFD, MS-DOS, Packer, Doug Belkofer, 1:234/10",
    0x52,   "Yuppie!, MS-DOS, Packer, Leo Moll, 2:242/2",
    0x53,   "EMMA, MS-DOS, Packer, Johan Zwiekhorst, 2:292/100",
    0x54,   "QBoxMail, QDOS, Packer/mailer, Jan Bredenbeek, 2:283/500",
    0x55,   "Number 4, MS-DOS, Packer/mailer, Ola Garstad, 2:502/15",
    0x56,   "Number 5, MS-DOS, Packer/mailer, Ola Garstad, 2:502/15",
    0x57,   "GSBBS, MS-DOS, Packer, Michaelangelo Jones, 1:260/209",
    0x58,   "Merlin, MS-DOS, Packer/mailer, Mark Lewis, 2:258/25",
    0x59,   "TPCS, MS-DOS, Packer, Mikael Kjellstrom, 2:201/211",
    0x5A,   "Raid, MS-DOS, Packer, George Peace, 1:270/101",
    0x5B,   "Outpost, MS-DOS, Packer/mailer, Mike Dailor, 1:260/236",
    0x5C,   "Nizze, MS-DOS, Packer, Tomas Nielsen, 2:205/202",
    0x5D,   "Armadillo, Macintosh, Packer, Erik Sea, 1:221/109",
    0x5E,   "rfmail, Unix, Packer/mailer, Heikki Suonsivu, 2:504/1",
    0x5F,   "Msgtoss, MS-DOS, Packer, Mike Zakharoff, 1:343/36",
    0x60,   "InfoTex, MS-DOS, Packer/mailer, Jan Spooren, 2:295/20",
    0x61,   "GEcho, MS-DOS, Packer, Gerard van der Land, 2:283/1.5",
    0x62,   "CDEhost, MS-DOS, Packer, Dennis D'Annunzio, 1:379/28",
    0x63,   "Pktize, MS-DOS, Packer, Joaquim Homrighausen, 2:270/17",
    0x64,   "PC-RAIN, MS-DOS, Packer/mailer, Ray Hyder, 1:272/40",
    0x65,   "Truffle, MS-DOS/OS2, Mailer, Mike Rissa, 2:504/59",
    0x66,   "Foozle, Amiga, Packer, Peer Hasselmeyer, 2:247/4",
    0x67,   "White Pointer, Macintosh, Packer/mailer, Alastair Rakine, 3:680/820",
    0x68,   "GateWorks, MS-DOS, Packer, Jamie Penner, 1:153/1025",
    0x69,   "Portal of Power, MS-DOS, Mailer, Soren Ager, 2:231/12",
    0x6A,   "MacWoof, Macintosh, Packer/mailer, Craig Vaughan, 1:109/342",
    0x6B,   "Mosaic, MS-DOS, Packer, Christopher King, 1:103/315",
    0x6C,   "TPBEcho, MS-DOS, Packer, Gerd Qualmann, 2:242/1",
    0x6D,   "HandyMail, MS-DOS, Packer/mailer, jim nutt, 1:114/30",
    0x6E,   "EchoSmith, MS-DOS, Packer, Noel Crow, 1:170/409",
    0x6F,   "FileHost, MS-DOS, Packer, Mark_Cole, 2:252/186",
    0x70,   "SFScan, MS-DOS, Packer, Bruce Anderson, 1:342/106",
    0x71,   "Benjamin, MS-DOS, Packer/mailer, Stefan Graf, 2:245/4.5436",
    0x72,   "RiBBS, OS9 (COCO), Packer/mailer, Ron Bihler, 1:104/54",
    0x73,   "MP, MS-DOS, Packer, Ivan Leong, 6:600/28",
    0x74,   "Ping, MS-DOS, Packer, David Nugent, 3:632/348",
    0x75,   "Door2Europe, MS-DOS, Packer/mailer, Michaela Schoebel, 2:247/14",
    0x76,   "SWIFT, MS-DOS, Packer/mailer, Hanno van der Maas, 2:500/2",
    0x77,   "WMAIL, MS-DOS, Packer, Silvan Calarco, 2:334/11.2",
    0x78,   "RATS, MS-DOS, Packer, Jason DeCaro, 1:260/205",
    0x79,   "Harry the Dirty Dog, OS2, Mailer/packer, George Edwards, 3:632/340.7",
    0x7A,   "Maximus-CBCS, MS-DOS/OS2, Packer, Scott Dudley, 1:249/106",
    0x7B,   "SwifEcho, MS-DOS, Packer, Dana Bell, 1:3801/8",
    0x7C,   "GCChost, Amiga, Packer, Davide Massarenti, 2:332/505.3",
    0x7D,   "RPX-Mail, MS-DOS, Packer, Joerg Wirtgen, 2:241/4034",
    0x7E,   "Tosser, MS-DOS, Packer, Albert Ng, 6:700/185",
    0x7F,   "TCL, MS-DOS, Packer, Ulf Hedlund, 2:201/602",
    0x80,   "MsgTrack, MS-DOS, Packer, Andrew Farmer, 1:163/115",
    0x81,   "FMail, MS-DOS, Packer, Folkert Wijnstra, 2:282/310",
    0x82,   "Scantoss, MS-DOS, Packer, Michael Matter, 2:243/44.3443",
    0x83,   "Point Manager, Amiga, Packer, Mario Pacchiarotti, 2:335/12.33",
    0x85,   "Simplex, MS-DOS/OS2, Packer, Chris Laforet, 1:152/401",
    0x86,   "UMTP, MS-DOS, Packer, Byron Copeland, 1:272/26",
    0x87,   "Indaba, MS-DOS, Packer, Pieter Muller, 5:7102/11",
    0x88,   "Echomail Engine, MS-DOS, Packer, Joe Jared, 1:103/200",
    0x89,   "DragonMail, OS2, Packer, Patrick O'Riva, 1:143/37",
    0x8A,   "Prox, MS-DOS, Packer, Gerhard Hoogterp, 2:283/1.2",
    0x8B,   "Tick, MS-DOS/OS2, Packer, Barry Geller, 1:266/12",
    0x8C,   "RA-Echo, MS-DOS, Packer, Roger Kirchhoff, 2:245/4",
    0x8D,   "TrapToss, Amiga, Packer, Maximilian Hantsch, 2:310/6",
    0x8E,   "Babel, MS-DOS/OS2, Packer, Jorgen Abrahamsen, 2:230/100.9",
    0x8F,   "UMS, Amiga, Packer, Martin Horneffer, 2:242/7.9",
    0x90,   "RWMail, MS-DOS, Packer, Remko Westrik, 2:285/309.5",
    0x91,   "WildMail, MS-DOS, Packer, Derek Koopowitz, 1:161/502",
    0x92,   "AlMAIL, MS-DOS, Packer, Alan Leung, 1:348/207",
    0x93,   "XCS, MS-DOS, Packer, Rudi Kusters, 2:512/34.4",
    0x94,   "Fone-Link, MS-DOS, Packer/mailer, Bryan Socha, 1:269/121",
    0x95,   "Dogfight, MS-DOS, Packer, Chris Tyson, 2:256/36",
    0x96,   "Ascan, MS-DOS, Packer, Arjen van Loon, 2:281/1.397",
    0x97,   "FastMail, MS-DOS, Packer, Jan Berends, 2:282/5",
    0x98,   "DoorMan, MS-DOS, Mailer, Christopher Dean, 1:105/70",
    0x99,   "PhaedoZap, Atari ST, Packer, Jeff Mitchell, 1:229/422",
    0x9A,   "SCREAM, MS-DOS, Packer/mailer, Jem Miller, 1:147/33",
    0x9B,   "MoonMail, MS-DOS, Packer/mailer, Hasse Wigdahl, 2:206/101",
    0x9C,   "Backdoor, Sinclair QL, Packer, Erik Slagter, 2:283/500.3",
    0x9D,   "MailLink, Archimedes, Packer/mailer, Jan-Jaap v. d. Geer, 2:500/133.1138",
    0x9E,   "Mail Manager, MS-DOS, Packer, Andreas Brodowski, 2:241/4006",
    0x9F,   "Black Star, Xenix 386, Packer/mailer, Jac Kersing, 2:283/333",
    0xA0,   "Bermuda, Atari ST/MS-DOS, Packer, Jac Kersing, 2:283/333",
    0xA1,   "PT, MS-DOS, Packer/mailer, Jerry Andrew, 1:109/426",
    0xA2,   "UltiMail, MS-DOS, Mailer, Brett Floren, 1:363/1000",
    0xA3,   "GMD, MS-DOS, Packer, John Souvestre, 1:396/1",
    0xA4,   "FreeMail, MS-DOS, Packer, Chad Nelson, 1:109/536",
    0xA5,   "Meliora, MS-DOS, Packer, Erik van Riper, 1:107/230",
    0xA6,   "Foodo, CPM-80, Packer/mailer, Ron Murray, 3:690/640.7",
    0xA7,   "MSBBS, CPM-80, Packer, Marc Newman, 1:106/601",
    0xA8,   "Boston BBS, MS-DOS, Packer/mailer, Tom Bradford, 1:101/625",
    0xA9,   "XenoMail, MS-DOS, Packer/mailer, Noah Wood, 1:284/14",
    0xAA,   "XenoLink, Amiga, Packer/mailer, Jonathan Forbes, 1:250/642",
    0xAB,   "ObjectMatrix, MS-DOS, Packer, Roberto Ceccarelli, 2:332/305.1",

    0,      "???"
    };

const n_ptab = sizeof(ptab) / sizeof(pname);

char *
prod_name( uint c )
	{
	int	i;

	for( i = 0; i < n_ptab; i++ )
		if( ptab[i].code == c )
			return ptab[i].name;

	return "Unknown";
	}






