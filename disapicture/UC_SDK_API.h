#ifndef UC_SDK_API_H
#define UC_SDK_API_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	VGA_FIRSTWINDOW = 0,
	VGA_SECONDWINDOW,
	TV_FIRSTWINDOW
};

enum
{
	DISPLAY_VGA = 0,
	DISPLAY_TV
};

enum
{
	HDMIOUTPUTMODE_480 = 0,
	HDMIOUTPUTMODE_720,
	HDMIOUTPUTMODE_1080
};

typedef struct sRECT
{
    int x;
    int y;
    unsigned int width;
    unsigned int height;
} sRECT;


typedef struct bsurface_memory
{
	void *buffer; /* pointer to mapped memory. Directly accessible by the application.
						This must be typecasted to the correct pixel format, for example
						unsigned short * for a 16 bit pixel format or unsigned long *
						for a 32 bit pixel format. */
	unsigned int pitch; /* The memory width of the surface in bytes. The visible width
							is <= pitch. Pitch must be used to calculate the next
							line of the surface. See the example in the bsurface_get_memory
							documentation. */
} bsurface_memory;


typedef struct bsurface_create_settings
{
	unsigned int pixel_format;	/* format of the surface. */
	unsigned int width;						/* visible width of the surface. The pitch
											may be wider. */
	unsigned int height;					/* visible height of the surface. */

	unsigned int alignment; /* optional buffer alignment specified
				 * as a power of 2, measured in bytes.
				 * 0 is default alignment (default), 1
				 * is 2-byte aligned, 2 is 4-byte
				 * aligned, etc. */

	unsigned int pitch; /* optional buffer pitch, measured in
			     * bytes. 0 is the default pitch (width *
			     * sizeof(pixel)). */
} bsurface_create_settings;


typedef struct bdecode_window_settings
{
    sRECT position;      /* Position and size on the display. Use display-relative coordinates. */
    unsigned int visible;                /* Is the window visible? */
    unsigned int zorder;             /* 0 is on bottom. For two decode window systems, 1 is on top. */
} bdecode_window_settings;

typedef struct bsurface_settings
{
	sRECT position; /* Position and size of the surface.
		position.width and height are readonly */
	unsigned int visible; /* Defaults to false. On magnum platforms, this causes a 
		surface to be automatically blitted to the framebuffer when bgraphics_sync
		is called. On legacy platforms, this causes the surface to be blended into
		the display by the compositor. */
	unsigned char alpha; /* 0x00 to 0xFF where 0xFF is opaque and 0x00 is transparent. 
		On magnum platforms, this is only used when automatically blitting to 
		the framebuffer with bgraphics_sync. On legacy platforms, this controls
		the alpha blending with the compositor. */
} bsurface_settings;


//����˵��: ���г�ʼ�����������ɵ�����ӿ���
//����˵��:
//����ֵ: ��
void TH_KTV_sys_init();

//����˵��: ������Ƶ�����ʽ
//����˵��:
//	nMode: 0:N�� 1:N/P �Զ��л�
//����ֵ: ��
void TH_MpegDecoderNtscPalSwitch(int nMode);//0:n  1:n/p  //��ʼ��ʱ���õ� ���������ʽ

//����˵��: ����HDMI����ֱ���
//����˵��:
//	nOutPutMode: 0:480 1:720 2:1080
//����ֵ: ��
void TH_SetHDMIOutPutMode(int nOutPutMode);


//����˵��: �����Ƶ�Ĵ�����Ϣ
//����˵��:
//	nWindowIndex: ��������һ����Ƶ���ڣ�
//����ֵ: ��
void TH_GetDecodeWindow(int nWindowIndex, bdecode_window_settings *settings);

//����˵��: ������Ƶ�Ĵ�����Ϣ
//����˵��:
//	nWindowIndex: ��������һ����Ƶ���ڣ�
//	settings: ��Ƶ������Ϣ�ṹ
//����ֵ: ��
void TH_SetDecodeWindow(int nWindowIndex, bdecode_window_settings *settings);

/////////////////////////����3D������µĽӿ�//////////////////////////////

//����˵��: 3D Open ��ʾ��ʼ��
//����˵��:
//	iDisplayID: ��������һ��DISPLAY, 0:VGA  1:TV
//����ֵ: DisplayHandle

unsigned long TH_GetDisplayHandle(int iDisplayID);

//����˵��: OPENGL ��ʾ��ʼ��
//����˵��:
//	nDisplay: ��������һ��DISPLAY, 0:VGA  1:TV
//	nSurface: Surface
//����ֵ: native_window
void *TH_CreateNativeWindow(void);

//����˵��: ������Ƶ�Ĵ�����Ϣ
//����˵��:
//	nDisplay: ��������һ��DISPLAY, 0:VGA  1:TV
//	nSurface: Surface
//����ֵ: ��
void TH_SetGraphicsFramebuffer(int nDisplayIndex, int nSurface);

//����˵��: ����Surface
//����˵��:
//	nWidth: Surface�Ŀ��
//	nHight: Surface�ĸ߶�
//����ֵ: Surface�ľ��, ����0: ����ɹ�   ����0: ����ʧ��
int TH_CreateSurface(int nWidth, int nHight);

//����˵��: �ͷ�Surface
//����˵��:
//	nSurface: Surface���
//����ֵ: ��
void TH_DestorySurface(int nSurface);

//����˵��: ���Surface����Ϣ
//����˵��:
//	nSurface: Surface���
//	settings: ���Surface��Ϣ�Ľṹָ��
//����ֵ: ��
void TH_GetSurfaceInfo(int nSurface, bsurface_settings *settings);

//����˵��: ���Surface��bsurface_memory�ṹ
//����˵��:
//	nSurface: Surface���
//	memory:	���Surface��bsurface_memory�ṹָ��
//����ֵ: ��
void TH_GetSurfaceMemory(int nSurface, bsurface_memory *memory);

//����˵��: surface���������̶���ɫֵ
//����˵��:
//	nSurface: Surface���
//	rect: �������
//����ֵ: 0��ʧ��  1���ɹ�
int TH_SurfaceFill(int nSurface,  const sRECT *rect, unsigned int pixel);

//����˵��: surface�����򿽱�
//����˵��:
//	destsurface: Ŀ��Surface
//	destrect: Ŀ������
//	srcsurface: ԴSurface
//	destrect: Դ����
//����ֵ: 0��ʧ��  1���ɹ�
int TH_SurfaceCopy(int destsurface, const sRECT *destrect, int srcsurface, const sRECT *srcrect);

//����˵��: surface���(�÷��뿴�ӿ������ע��)
//����˵��:
//	destsurface: Ŀ��Surface
//	destrect: Ŀ������
//  operation: ��ϲ���
//	src1: ԴSurface1
//	src1_rect: Դ����1
//	src2: ԴSurface2
//	src2_rect: Դ����2
//	pixel1: ԴSurface2
//	pixel2: Դ����2
//����ֵ: 0��ʧ��  1���ɹ�
int TH_SurfaceBlit(int destsurface, const sRECT *destrect, unsigned int operation, int src1, const sRECT *src1_rect, int src2, const sRECT *src2_rect, unsigned int pixel1, unsigned int pixel2);

/**
BSURFACE_BLEND_XXXX are values to be used in the bsurface_blit operation
parameter. You should chose one value.

Alpha values are always relative to src1 and should be between 0x00 and 0xFF.
Therefore alpha of 0xFF means take all of src1, none of src2. 
Likewise alpha of 0x00 means take none of src1, all of src2.

There are some special cases which are enforced and optimized in the implementation.
For instance, if you don't want to blend colors, you can set src2 to NULL,
use BSURFACE_BLEND_WITH_SRC1_ALPHA, and set pixel1 == 0xFF, and the color blend
will be bypassed. You can still use the BSURFACE_SET_DEST_ALPHA to manipulate
the alpha-per-pixel.
**/	

/* Use the pixel1 parameter to blend the sources. */
#define BSURFACE_BLEND_WITH_PIXEL1			0x0000
/* Copy src1's alpha-per-pixel to blend the sources */
#define BSURFACE_BLEND_WITH_SRC1_ALPHA		0x0001
/* Copy src2's alpha-per-pixel to blend the sources */
#define BSURFACE_BLEND_WITH_SRC2_ALPHA		0x0002
/* Blend two palettized surfaces using colorkey to select per pixel */
#define BSURFACE_SELECT_PALETTE_WITH_COLORKEY 0x0003
/* Copy source (blend with one) */
#define BSURFACE_BLEND_WITH_ONE		        0x0004

/**
BSURFACE_BLIT_DEST_ALPHA_XXX specifies how the alpha-per-pixel values of the dest
surface should be filled. If the dest has no alpha-per-pixel, any setting is ignored.
**/

/* Copy the pixel2 parameter into the dest's alpha-per-pixel. */
#define BSURFACE_SET_DEST_ALPHA_WITH_PIXEL2			0x0000
/* Copy src1's alpha channel into dest's alpha-per-pixel. */
#define BSURFACE_SET_DEST_ALPHA_WITH_SRC1_ALPHA		0x0010
/* Copy src2's alpha channel into dest's alpha-per-pixel. */
#define BSURFACE_SET_DEST_ALPHA_WITH_SRC2_ALPHA		0x0020
/* Blend src1's and src2's alpha-per-pixel into dest's alpha-per-pixel. 
The formula is dest = src1 + src2 * (1-src1). */
#define BSURFACE_SET_DEST_ALPHA_WITH_AVG_ALPHA		0x0030
/* Blend pixel2 parameter and src2's alpha-per-pixel into dest's alpha-per-pixel. 
The formula is dest = pixel2 + src2 * (1-pixel2). */
#define BSURFACE_SET_DEST_ALPHA_WITH_AVG_ALPHA_PIXEL2   0x0040
 /* Multiply src1's alpha-per-pixel with pixel2 parameter. 
The formula is dest = src1 * pixel2. */
#define BSURFACE_SET_DEST_ALPHA_WITH_SRC1_TIMES_PIXEL2  0x0050

/**
Summary:
Blit two surfaces together into a destination surface.

Desription:
Blit is a superset of copy. It gives you more control over how two surfaces
will be blended together and how the destination surface's alpha-per-pixel (or
alpha channel) will be populated if present.

src1 or src2 can be the same as dest. 

If src2 is NULL, then you must use BSURFACE_BLEND_WITH_SRC1_ALPHA and
have pixel1 of 0xFF. Anything else implies that you are blending between src1
and src2, which cannot be done. Likewise for src1.

If both src1 and src2 are null, then operation is ignored and pixel1 is regarded
as a fill color.

There are some operations that will internally result as a two-pass operation.
This is hardware specific. In general, the Settop API will fulfill your request
or return an error code if it cannot. Please see the implementation for more 
details.

Scaling and clipping are specified by the proportion of the src1, src2 and
dest rectangles. The entire src1_rect is combined with the entire src2_rect and 
the result is copied into the entire dest_rect. Some platforms have
scaling limitations. For instance, on 7038 src2 cannot scale, therefore its
width and height must match dest's.

Example:
	// bsurface_copy is equivalent to:
	bsurface_blit(destsurface, destrect,
		BSURFACE_BLEND_WITH_PIXEL1|BSURFACE_SET_DEST_ALPHA_WITH_SRC1_ALPHA,
		src1, src1rect, NULL, NULL, 0xFF, 0xFF);

	// blend two surfaces together using a constant alpha, and set
	// the dest's alpha channel with a constant alpha
	bsurface_blit(destsurface, destrect,
		BSURFACE_BLEND_WITH_PIXEL1|BSURFACE_SET_DEST_ALPHA_WITH_PIXEL2,
		src1, src1rect, src2, src2rect, 0xA0, 0xFF);

	// blend two surfaces together using src1's alpha channel, and set
	// the dest's alpha channel to be src1's alpha. The pixel1 and pixel2
	// values are ignored.
	bsurface_blit(destsurface, destrect,
		BSURFACE_BLEND_WITH_SRC1_ALPHA|BSURFACE_SET_DEST_ALPHA_WITH_SRC1_ALPHA,
		src1, src1rect, src2, src2rect, 0, 0);
**/

/////////////////////////////// ����Ƶ�ӿ� //////////////////////////////////////////////////////////////// decode_id 0:��һ·��Ƶ 1:�ڶ�·��Ƶ

//����˵��: ������Ƶ���Ϊ��һ·���������
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��ʧ��  1���ɹ�
void TH_MpegDecoderSetAudioOutput(int nDecodeID);

//����˵��: ��ʼ������Ƶ
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//  nMode:  ����ģʽ 1����ģʽ����  0������ģʽ����
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPlay(int nDecodeID, int nMode);

//����˵��: ����������Ƶ
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderStop(int nDecodeID);

//����˵��: ��ý��뻺����
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//  p:  ������ָ��
//  dwBufLen: ��õĻ������Ĵ�С(�����ͣ�˵Ļ�����û������Ĵ�СӦ��Ϊ0)
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderGetBuffer(int nDecodeID, unsigned char** p, int* dwBufLen);

//����˵��: ���Ž��뻺�����е�����
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//  p:  ������ָ��
//  dwBufLen: �����������ݵĴ�С
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPushBuffer(int nDecodeID, unsigned char* p, int dwBufLen);

//����˵��: ������Ż�����������
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderFlushBuffer(int nDecodeID);

//����˵��: �жϻ������������Ƿ���
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��û�в������  1���������
int TH_MpegDecoderWaitingForPlayEnd(int nDecodeID);//�жϻ������������Ƿ��� 0:û�в���  1:������

//����˵��: �����Ƶ�Ĳ���ʱ��
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderGetVideoPts(int nDecodeID,unsigned long *pPTS);

//����˵��: �����Ƶ�Ĳ���ʱ��
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderGetAudioPts(int nDecodeID,unsigned long *pPTS);

//����˵��: ��ͣ
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPause(int nDecodeID);

//����˵��: ����
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderResume(int nDecodeID);//

//����˵��: ��õ�ǰ������Ƶ��������
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//����ֵ: ������
int TH_MpegDecoderGetAudioStreamNumber(int nDecodeID);

//����˵��: �����������������
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//  dwMode:  0:������ 1:������  2:������
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderSetAudioMode(int nDecodeID,int dwMode);

//����˵��: ������������������0��ʼ
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//  nStreamIndex:  ��������
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderSelectAudioStream(int nDecodeID,int nStreamIndex);

//����˵��: �������������0-100��
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//  nVol:  ����ֵ��0-100��
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderSetVolume(int nDecodeID,int nVol);

//����˵��: ���þ���������
//����˵��:
//	nDecodeID: �ڼ�·��Ƶ0:��һ·��Ƶ 1:�ڶ�·��Ƶ
//  nMute:  1:���� 0:����
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderSetMute(int nDecodeID,int nMute);
/////////////////////////////// ����Ƶ�ӿ� //////////////////////////////////////////////////////////////////

//¼���طŽӿ�//

//����˵��: ����¼������
//����˵��:
//	nVolume: ¼������
//����ֵ: 0��ʧ��  1���ɹ�
int TH_SetRecordVolume(int nVolume);

//����˵��: ��ʼ¼��
//����˵��:
//����ֵ: 0��ʧ��  1���ɹ�
int TH_DecoderAnalogAudioStart();

//����˵��: ��ȡ������
//����˵��:
//����ֵ: �����ʴ�С
int TH_Get_Audio_Sample_Rate();

//����˵��: ���¼�����ݻ�����
//����˵��:
//  pBuffer: ¼�����ݻ�����ָ��
//  p:  ¼�����ݴ�С
//����ֵ: 0��ʧ��  1���ɹ�
int TH_GetRecordBuf(char ** pBuffer, int* p);

//����˵��: ��ȡ¼�����ݵ���������
//����˵��:
//  nSize: ¼�����ݴ�С
//����ֵ: 0��ʧ��  1���ɹ�
int TH_PCMRecordReadComplete(int nSize);

//����˵��: ¼�����
//����˵��:
//����ֵ: 0��ʧ��  1���ɹ�
int TH_DecoderAnalogAudioStop();


//�ط� ���Ż���Ҫ֧�� wav �� MP3 ���ָ�ʽ�������ǲ���wav�Ľӿ�

//����˵��: ���û�������
//����˵��:
//  nSample_rate: ������
//  nBits_per_sample: ÿ��������λ��
//  nChannels:  ������
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPlayWavSetParams(int nSample_rate,int nBits_per_sample, int nChannels);

//����˵��: ���û�������
//����˵��:
//  nVol: ��������
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPlayWavSetVolume(int nVol);


//����˵��: ��ʼ����
//����˵��:
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPlayWavStart();

//����˵��: ��û����Ļ�����ָ��ͻ���������
//����˵��:
//  buffer: �������ݻ�����ָ��
//  p:  �������ݻ�������С
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPlayWavGetBuf(char **buffer, int *p);

//����˵��: ���Ż����Ļ���������
//����˵��:
//  nLen: �������ݳ���
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPlayWavPushBuf(int nLen);//֪ͨ�����豸�����Ļ����������Ƕ���

//����˵��: ֹͣ����
//����˵��:
//����ֵ: 0��ʧ��  1���ɹ�
int TH_MpegDecoderPlayWavStop();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////����3D������µĽӿ�//////////////////////////////
/*��ȫ���glTexImage2D�ĺ�����ǰ��Ĳ�����glTexImage2Dһ����
surface������һ��surface�����ɵ���������һ�ݸ�surface��ͼ������
FunCallback������glTexImage2D����
*/
typedef void (*glTexImage2D_Callback)(unsigned int target, int level, int internalformat, int width, int height, int border, unsigned int  format, unsigned int type, void* datas);
void private_glTexImage2D (unsigned int target, int level, int internalformat, int width, int height, int border, unsigned int  format, unsigned int type, int surface, glTexImage2D_Callback FunCallback);


typedef void (*glTexSubImage2D_Callback)(unsigned int target, int level, int xoffset, int yoffset, int width, int height, unsigned int format, unsigned int type, void *pixels);
void private_glTexSubImage2D (unsigned int target, int level, int xoffset, int yoffset, int width, int height, int border, unsigned int  format, unsigned int type, int surface, glTexSubImage2D_Callback FunCallback);

#ifdef __cplusplus
}
#endif

#endif 


