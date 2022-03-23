#include <stdio.h>
#include <stdlib.h>

#include "lame/lame.h"


#define PCM_SAMPLE_CNT 	(4096)


#define PCM_BUF_SIZE 	(PCM_SAMPLE_CNT * sizeof(short))
#define MP3_BUF_SIZE 	(PCM_SAMPLE_CNT * sizeof(short))


int main(int argc, char *argv[])
{
	char *pcmFileName = NULL;
	char *mp3FileName = NULL;
	FILE *fpPcm = NULL;
	FILE *fpMp3 = NULL;
	int readSampleCnt = -1;
	int encBytes = -1;
	int samplerate = -1;
	int channels = -1;	
	short pcmBuf[PCM_BUF_SIZE];
	unsigned char mp3Buf[MP3_BUF_SIZE];
	lame_t mp3EncHandler = NULL;

	if(argc != 5)
	{
		printf("Usage: \n"
			   "    %s <in pcm file> <smaplerate> <channels> <out mp3 file>\n"
			   "Examples: \n"
			   "    %s audio/test1_44100_16bit_stereo.pcm 44100 2 out1_44100_stereo.mp3\n"
			   "    %s audio/test2_22050_16bit_stereo.pcm 22050 2 out2_22050_stereo.mp3\n"
			   "    %s audio/test3_22050_16bit_mono.pcm   22050 1 out3_22050_mono.mp3\n"
			   "    %s audio/test4_8000_16bit_mono.pcm     8000 1 out4_8000_mono.mp3\n",
			   argv[0], argv[0], argv[0], argv[0], argv[0]);
		return -1;
	}
	else
	{
		pcmFileName = argv[1];
		samplerate  = atoi(argv[2]);
		channels    = atoi(argv[3]);
		mp3FileName = argv[4];
	}

	/* open file */
	fpPcm = fopen(pcmFileName, "rb");
	fpMp3 = fopen(mp3FileName, "wb");
	if(!fpPcm || !fpMp3)
	{
		printf("[%s:%d]: open %s or %s failed!\n", __FUNCTION__, __LINE__, pcmFileName, mp3FileName);
		return -1;
	}

	/* mp3 encode 1/5 : open encoder */
	mp3EncHandler = lame_init();
	if(!mp3EncHandler)
	{
		printf("[%s:%d] open MP3 encoder failed!\n", __FUNCTION__, __LINE__);
		goto exit;
	}

	/* mp3 encode 2/5 : configure encoder params */
	lame_set_in_samplerate(mp3EncHandler, samplerate); 		/* default = 44100hz */
	//lame_set_out_samplerate(mp3EncHandler, samplerate); 	/* if not set, it will be auto configured */
	lame_set_num_channels(mp3EncHandler, channels); 		/* number of channels in input stream. default=2  */
	//lame_set_mode(mp3EncHandler, STEREO); 				/* if not set, it will be auto configured */
	lame_set_VBR(mp3EncHandler, vbr_mtrh); 					/* Types of VBR. vbr_default=vbr_mtrh */
	//lame_set_brate(mp3EncHandler, 160; 					/* for CBR */
	//lame_set_VBR_mean_bitrate_kbps(mp3EncHandler, 160); 	/* Ignored except for VBR=vbr_abr (ABR mode) */
    lame_init_params(mp3EncHandler);

	while(1)
	{
		readSampleCnt = fread(pcmBuf, sizeof(short)/* 16bit */, PCM_SAMPLE_CNT, fpPcm);
		if(readSampleCnt == PCM_SAMPLE_CNT)
		{
			if(channels == 1)
			{
				/* mp3 encode 3/5 : encode(mono) */
				//encBytes = lame_encode_buffer(mp3EncHandler, NULL, pcmBuf, readSampleCnt/channels, mp3Buf, MP3_BUF_SIZE); /* R */
				encBytes = lame_encode_buffer(mp3EncHandler, pcmBuf, NULL, readSampleCnt/channels, mp3Buf, MP3_BUF_SIZE); /* L */
			}
			else if(channels == 2)
			{
				/* mp3 encode 3/5 : encode(stereo) */
				encBytes = lame_encode_buffer_interleaved(mp3EncHandler, pcmBuf, readSampleCnt/channels, /* number of samples per channel, not number of samples in pcmBuf*/
																	 mp3Buf, MP3_BUF_SIZE);
			}
			else
			{
				printf("[%s:%d] Unknown channels(%d)!\n", __FUNCTION__, __LINE__, channels);
				goto exit;
			}

			if(encBytes > 0)
			{
				/* save to file */
				fwrite(mp3Buf, 1, encBytes, fpMp3);
				printf("<in> [pcm bytes: %d]   |   <out> [mp3 bytes: %d]\n", (int)(readSampleCnt * sizeof(short)), encBytes);
			}
			else
			{
				printf("[%s:%d] encode failed!\n", __FUNCTION__, __LINE__);
				goto exit;
			}
		}
		else
		{
			break;
		}
	}

	/* mp3 encode 4/5 : flush to file */
	encBytes = lame_encode_flush(mp3EncHandler, mp3Buf, MP3_BUF_SIZE);
	if(encBytes > 0)
	{
		/* save to file */
		fwrite(mp3Buf, 1, encBytes, fpMp3);
		printf("<in> [pcm bytes: %d]   |   <out> [mp3 bytes: %d]\n", (int)(readSampleCnt * sizeof(short)), encBytes);
	}
	else
	{
		printf("[%s:%d] encode failed!\n", __FUNCTION__, __LINE__);
		goto exit;
	}

	printf("\e[32m%s -> %s success!\e[0m\n", argv[1], argv[4]);

exit:
	/* mp3 encode 5/5 : close encoder */
	if(mp3EncHandler) lame_close(mp3EncHandler);

	if(mp3EncHandler) {fflush(fpMp3); fclose(fpMp3);}
	if(mp3EncHandler) fclose(fpPcm);

	return 0;
}
