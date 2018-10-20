#include <stdio.h>
#include <string.h>
#include "function.h"
#include "opt.h"

#define BSIZE 512

typedef struct
{
	unsigned long	start_code;		/* B instruction */
	unsigned char	logo[0xA0-0x04];	/* logo data */
	char			title[0xC];		/* game title name */
	unsigned long	game_code;		
	unsigned short	maker_code;		
	unsigned char	fixed;			/* 0x96 */
	unsigned char	unit_code;		/* 0x00 */
	unsigned char	device_type;	/* 0x80 */
	unsigned char	unused[7];		
	unsigned char	game_version;	/* 0x00 */
	unsigned char	complement;		/* 800000A0..800000BC */
	unsigned short	checksum;		/* 0x0000 */
} Header;


static const Header good_header = {
	/* start_code */
	0xEA00002E,\
	/* logo */
	{ 0x24,0xFF,0xAE,0x51,0x69,0x9A,0xA2,0x21,0x3D,0x84,0x82,0x0A,0x84,0xE4,0x09,0xAD,\
	0x11,0x24,0x8B,0x98,0xC0,0x81,0x7F,0x21,0xA3,0x52,0xBE,0x19,0x93,0x09,0xCE,0x20,\
	0x10,0x46,0x4A,0x4A,0xF8,0x27,0x31,0xEC,0x58,0xC7,0xE8,0x33,0x82,0xE3,0xCE,0xBF,\
	0x85,0xF4,0xDF,0x94,0xCE,0x4B,0x09,0xC1,0x94,0x56,0x8A,0xC0,0x13,0x72,0xA7,0xFC,\
	0x9F,0x84,0x4D,0x73,0xA3,0xCA,0x9A,0x61,0x58,0x97,0xA3,0x27,0xFC,0x03,0x98,0x76,\
	0x23,0x1D,0xC7,0x61,0x03,0x04,0xAE,0x56,0xBF,0x38,0x84,0x00,0x40,0xA7,0x0E,0xFD,\
	0xFF,0x52,0xFE,0x03,0x6F,0x95,0x30,0xF1,0x97,0xFB,0xC0,0x85,0x60,0xD6,0x80,0x25,\
	0xA9,0x63,0xBE,0x03,0x01,0x4E,0x38,0xE2,0xF9,0xA2,0x34,0xFF,0xBB,0x3E,0x03,0x44,\
	0x78,0x00,0x90,0xCB,0x88,0x11,0x3A,0x94,0x65,0xC0,0x7C,0x63,0x87,0xF0,0x3C,0xAF,\
	0xD6,0x25,0xE4,0x8B,0x38,0x0A,0xAC,0x72,0x21,0xD4,0xF8,0x07 },\
	/* title */
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 } ,\
	/* game code */
	0x00000000,\
	/* maker code */
	0x3130,\
	/* fixed */
	0x96,\
	/* unit_code */
	0x00,\
	/* device type */
	0x80,\
	/* unused */
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00 } ,\
	/* game version */
	0x00,\
	/* complement */
	0x00,\
	/* checksum */
	0x0000
};

enum Error{
	err_fin,
	err_fout,
	err_gamecode,
	err_makercode,
	err_version,
	err_dfout,
	err_dfin,
	err_debug
};

static const char *errstr[]={
"can not access Input File",\
"can not access Output File",\
"size of game code",\
"size of maker code",\
"game version",\
"double output file",\
"double input file",\
"Debug entry point",\
NULL};


static int printerr(int id,const char **errstr)
{
	fprintf(stderr,"\nError id:%d => %s\n",id,errstr[id]);
	return 1;
}

static const char *croppath(const char *path)
{
	const char *cpath;
	for(cpath=path;*path;++path) if(*path == '/' || *path=='\\') cpath=path+1;

	return cpath;
}

static int showhelp(const char *pname,const char *opt[],const char *optstr[])
{
	size_t i;
	fprintf(stderr,"\n%s is a ROM GameBoy Advance Head Builder & Fixer\n",pname);
	fprintf(stderr,"\nUSAGE:: %s [option] infile\n\n",pname);

	for(i=0;opt[i];++i)
		fprintf(stderr,"%s\t => %s\n",opt[i],optstr[i]);

	return 1;
}

static const char* opt[] = { "-g","-a", "-p", "-t:","-c:","-m:","-r:","-o:","-d:", NULL };
static const char* optstr[] = { 
"Replaced by good header","Add header to an output file",\
 "Pad to next exact power of 2. No minimum size",\
"Patch title. Fill zero if none given",\
"Patch game code (four characters)",\
"Patch maker code (two characters)",\
"Patch game version (number)",\
"Output file (must be assigned)",\
"Degug enabled (0 or 1)", NULL };

enum {
	opt_g,
    opt_a,
    opt_p,
    opt_t,
	opt_c,
	opt_m,
	opt_r,
	opt_o,
	opt_d
};

static unsigned char HeaderComplement(const Header *header)
{
	unsigned char c = 0;
	const unsigned char *p = (const unsigned char *)header + 0xA0;
	unsigned int n;
	for (n=0xA0; n<=0xBC; n++)
		c += *p++;

	return -(0x19+c);
}

static void filepadding(FILE *fout,int padval)
{
	size_t size ;
	size_t bit;

	size = ftell(fout);

	for (bit=32; bit>=1; bit--) if (size & (1<<(bit-1))) break;
	if (size != (1U<<(bit-1)))
	{
		size_t todo = (1U<<bit) - size;
		while (todo--) fputc(padval, fout);
	}
}

int main(int argc, const char *argv[])
{
	static Header header;
	static Header addheader=good_header;
	static char buff[BSIZE];
	const char *msg;

	FILE *fin=NULL,*fout=NULL;

	int isgood=0,isadd=0,ispadding=0,istitle=0,isgamecode=0,ismakercode=0,isversion=0,isdebug=0;

	if(argc==1)
		return showhelp(croppath(argv[0]),opt,optstr);

  {
        int i;
        unsigned int ui_cindex;

        for (ui_cindex = DSTART; (i = opt_action(argc, argv, opt, buff,
                                      BSIZE, DSTART))
             != e_optend;
             ui_cindex++) {

            switch (i) {

            case opt_g:

					isgood = 1;

                	break;

            case opt_a:

					isadd = 1;

                	break;

            case opt_p:

					ispadding =1;

                	break;

			case opt_t:
					if(*buff)
						memcpy(addheader.title, buff, sizeof(addheader.title));
					else
						memset(addheader.title, 0, sizeof(addheader.title));
				

					istitle=1;

					break;

			case opt_c:
					if(strlen(buff)!=sizeof(addheader.game_code))
					{
						if(fin) fclose(fin);
						if(fout) fclose(fout);

						return printerr(err_gamecode,errstr);
					}

					addheader.game_code = buff[0] | buff[1]<<8 | buff[2]<<16 | buff[3]<<24;

					isgamecode=1;

					break;

			case opt_m:
					if(strlen(buff)!=sizeof(addheader.maker_code))
					{
						if(fin) fclose(fin);
						if(fout) fclose(fout);

						return printerr(err_makercode,errstr);
					}
					addheader.maker_code = buff[0] | buff[1]<<8;

					ismakercode=1;

					break;

			case opt_r:
					if (!*buff || !isUint(buff))
					{
						if(fin) fclose(fin);
						if(fout) fclose(fout);

						return printerr(err_version,errstr);
					}

					addheader.game_version = s2ui(buff);

					isversion=1;

					break;

			case opt_o:
					if(fout)
					{
						if(fin) fclose(fin);
						if(fout) fclose(fout);

						return printerr(err_dfout,errstr);
					}

					fout=fopen(buff, "wb");

					break;

			case opt_d:
					if (!*buff || !isUint(buff))
					{
						if(fin) fclose(fin);
						if(fout) fclose(fout);

						return printerr(err_debug,errstr);
					}
					addheader.logo[0x9C-0x04] = 0xA5;
					addheader.device_type = ((s2ui(buff) & 1) << 7);

					isdebug=1;

					break;

            default:
					if(fin)
					{
						if(fin) fclose(fin);
						if(fout) fclose(fout);

						return printerr(err_dfin,errstr);
					}

					fin=fopen(buff, "rb");

					break;
            }
        }
    }

	if(!fin) 
	{
		if(fout) fclose(fout);

		return printerr(err_fin,errstr);
	}

	if(!fout) 
	{ 
		if(fin) fclose(fin);

		return printerr(err_fout,errstr); 
	}

	fread(&header, sizeof(header), 1, fin);
	rewind(fin);

	if(isgood)
	{
		static Header replaceheader = good_header;
		replaceheader.complement = HeaderComplement(&replaceheader);
		
		fwrite(&replaceheader, sizeof(replaceheader), 1, fout);

		msg="Header ROM Replaced by good one!";

		fseek(fin,sizeof(replaceheader),SEEK_CUR);

	}
	else if(isadd)
	{
		addheader.complement = HeaderComplement(&addheader);
		
		fwrite(&addheader, sizeof(addheader), 1, fout);

		msg="Header ROM Added!";
	}
	else
	{
		memcpy(header.logo, good_header.logo, sizeof(header.logo));
		memcpy(&header.fixed, &good_header.fixed, sizeof(header.fixed));
		header.device_type=good_header.device_type;

		if(istitle) memcpy(header.title,addheader.title,sizeof(header.title));

		if(isgamecode) header.game_code=addheader.game_code;

		if(ismakercode) header.maker_code=addheader.maker_code;

		if(isversion) memcpy(&header.game_version,&addheader.game_version,sizeof(header.game_version));

		if(isdebug) 
		{
			header.logo[0x9C-0x04]=addheader.logo[0x9C-0x04];
			header.device_type=addheader.device_type;
		}
			
		header.complement = 0;
		header.checksum = 0;
		header.complement = HeaderComplement(&header);

		fwrite(&header, sizeof(header), 1, fout);

		msg="Header ROM fixed!";

		fseek(fin,sizeof(header),SEEK_CUR);
	}

	{
		int ch;
		while((ch=fgetc(fin))!=EOF)
		fputc(ch,fout);
	}

	puts(msg);

	if(ispadding)
	{
		filepadding(fout,0xFF);

		puts("Header ROM padded!");		
	}

	fclose(fin);
	fclose(fout);

	return 0;
}
