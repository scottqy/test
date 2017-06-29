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


//功能说明: 所有初始化工作都集成到这个接口里
//参数说明:
//返回值: 无
void TH_KTV_sys_init();

//功能说明: 设置视频输出制式
//参数说明:
//	nMode: 0:N制 1:N/P 自动切换
//返回值: 无
void TH_MpegDecoderNtscPalSwitch(int nMode);//0:n  1:n/p  //初始化时候用的 电视输出制式

//功能说明: 设置HDMI输出分辨率
//参数说明:
//	nOutPutMode: 0:480 1:720 2:1080
//返回值: 无
void TH_SetHDMIOutPutMode(int nOutPutMode);


//功能说明: 获得视频的窗口信息
//参数说明:
//	nWindowIndex: 代表是哪一个视频窗口，
//返回值: 无
void TH_GetDecodeWindow(int nWindowIndex, bdecode_window_settings *settings);

//功能说明: 设置视频的窗口信息
//参数说明:
//	nWindowIndex: 代表是哪一个视频窗口，
//	settings: 视频窗口信息结构
//返回值: 无
void TH_SetDecodeWindow(int nWindowIndex, bdecode_window_settings *settings);

/////////////////////////关于3D纹理更新的接口//////////////////////////////

//功能说明: 3D Open 显示初始化
//参数说明:
//	iDisplayID: 代表是哪一个DISPLAY, 0:VGA  1:TV
//返回值: DisplayHandle

unsigned long TH_GetDisplayHandle(int iDisplayID);

//功能说明: OPENGL 显示初始化
//参数说明:
//	nDisplay: 代表是哪一个DISPLAY, 0:VGA  1:TV
//	nSurface: Surface
//返回值: native_window
void *TH_CreateNativeWindow(void);

//功能说明: 设置视频的窗口信息
//参数说明:
//	nDisplay: 代表是哪一个DISPLAY, 0:VGA  1:TV
//	nSurface: Surface
//返回值: 无
void TH_SetGraphicsFramebuffer(int nDisplayIndex, int nSurface);

//功能说明: 申请Surface
//参数说明:
//	nWidth: Surface的宽度
//	nHight: Surface的高度
//返回值: Surface的句柄, 大于0: 申请成功   等于0: 申请失败
int TH_CreateSurface(int nWidth, int nHight);

//功能说明: 释放Surface
//参数说明:
//	nSurface: Surface句柄
//返回值: 无
void TH_DestorySurface(int nSurface);

//功能说明: 获得Surface的信息
//参数说明:
//	nSurface: Surface句柄
//	settings: 存放Surface信息的结构指针
//返回值: 无
void TH_GetSurfaceInfo(int nSurface, bsurface_settings *settings);

//功能说明: 获得Surface的bsurface_memory结构
//参数说明:
//	nSurface: Surface句柄
//	memory:	存放Surface的bsurface_memory结构指针
//返回值: 无
void TH_GetSurfaceMemory(int nSurface, bsurface_memory *memory);

//功能说明: surface按区域填充固定颜色值
//参数说明:
//	nSurface: Surface句柄
//	rect: 填充区域
//返回值: 0：失败  1：成功
int TH_SurfaceFill(int nSurface,  const sRECT *rect, unsigned int pixel);

//功能说明: surface按区域拷贝
//参数说明:
//	destsurface: 目标Surface
//	destrect: 目标区域
//	srcsurface: 源Surface
//	destrect: 源区域
//返回值: 0：失败  1：成功
int TH_SurfaceCopy(int destsurface, const sRECT *destrect, int srcsurface, const sRECT *srcrect);

//功能说明: surface混合(用法请看接口下面的注释)
//参数说明:
//	destsurface: 目标Surface
//	destrect: 目标区域
//  operation: 混合参数
//	src1: 源Surface1
//	src1_rect: 源区域1
//	src2: 源Surface2
//	src2_rect: 源区域2
//	pixel1: 源Surface2
//	pixel2: 源区域2
//返回值: 0：失败  1：成功
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

/////////////////////////////// 音视频接口 //////////////////////////////////////////////////////////////// decode_id 0:第一路视频 1:第二路视频

//功能说明: 设置音频输出为哪一路解码的声音
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：失败  1：成功
void TH_MpegDecoderSetAudioOutput(int nDecodeID);

//功能说明: 开始播放视频
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//  nMode:  播放模式 1：简单模式播放  0：正常模式播放
//返回值: 0：失败  1：成功
int TH_MpegDecoderPlay(int nDecodeID, int nMode);

//功能说明: 结束播放视频
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：失败  1：成功
int TH_MpegDecoderStop(int nDecodeID);

//功能说明: 获得解码缓冲区
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//  p:  缓冲区指针
//  dwBufLen: 获得的缓冲区的大小(如果暂停了的话，获得缓冲区的大小应该为0)
//返回值: 0：失败  1：成功
int TH_MpegDecoderGetBuffer(int nDecodeID, unsigned char** p, int* dwBufLen);

//功能说明: 播放解码缓冲区中的数据
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//  p:  缓冲区指针
//  dwBufLen: 缓冲区中数据的大小
//返回值: 0：失败  1：成功
int TH_MpegDecoderPushBuffer(int nDecodeID, unsigned char* p, int dwBufLen);

//功能说明: 清除播放缓冲区的数据
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：失败  1：成功
int TH_MpegDecoderFlushBuffer(int nDecodeID);

//功能说明: 判断缓冲区的数据是否播完
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：没有播放完毕  1：播放完毕
int TH_MpegDecoderWaitingForPlayEnd(int nDecodeID);//判断缓冲区的数据是否播完 0:没有播完  1:播完了

//功能说明: 获得视频的播放时间
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：失败  1：成功
int TH_MpegDecoderGetVideoPts(int nDecodeID,unsigned long *pPTS);

//功能说明: 获得音频的播放时间
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：失败  1：成功
int TH_MpegDecoderGetAudioPts(int nDecodeID,unsigned long *pPTS);

//功能说明: 暂停
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：失败  1：成功
int TH_MpegDecoderPause(int nDecodeID);

//功能说明: 播放
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 0：失败  1：成功
int TH_MpegDecoderResume(int nDecodeID);//

//功能说明: 获得当前播放视频的音轨数
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//返回值: 音轨数
int TH_MpegDecoderGetAudioStreamNumber(int nDecodeID);

//功能说明: 单音轨歌曲设置声道
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//  dwMode:  0:左声道 1:右声道  2:立体声
//返回值: 0：失败  1：成功
int TH_MpegDecoderSetAudioMode(int nDecodeID,int dwMode);

//功能说明: 多音轨歌曲设置音轨从0开始
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//  nStreamIndex:  音轨索引
//返回值: 0：失败  1：成功
int TH_MpegDecoderSelectAudioStream(int nDecodeID,int nStreamIndex);

//功能说明: 设置输出音量（0-100）
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//  nVol:  音量值（0-100）
//返回值: 0：失败  1：成功
int TH_MpegDecoderSetVolume(int nDecodeID,int nVol);

//功能说明: 设置静音、放音
//参数说明:
//	nDecodeID: 第几路视频0:第一路视频 1:第二路视频
//  nMute:  1:静音 0:放音
//返回值: 0：失败  1：成功
int TH_MpegDecoderSetMute(int nDecodeID,int nMute);
/////////////////////////////// 音视频接口 //////////////////////////////////////////////////////////////////

//录音回放接口//

//功能说明: 设置录音音量
//参数说明:
//	nVolume: 录音音量
//返回值: 0：失败  1：成功
int TH_SetRecordVolume(int nVolume);

//功能说明: 开始录音
//参数说明:
//返回值: 0：失败  1：成功
int TH_DecoderAnalogAudioStart();

//功能说明: 获取采样率
//参数说明:
//返回值: 采样率大小
int TH_Get_Audio_Sample_Rate();

//功能说明: 获得录音数据缓冲区
//参数说明:
//  pBuffer: 录音数据缓冲区指针
//  p:  录音数据大小
//返回值: 0：失败  1：成功
int TH_GetRecordBuf(char ** pBuffer, int* p);

//功能说明: 读取录音数据到缓冲区中
//参数说明:
//  nSize: 录音数据大小
//返回值: 0：失败  1：成功
int TH_PCMRecordReadComplete(int nSize);

//功能说明: 录音完毕
//参数说明:
//返回值: 0：失败  1：成功
int TH_DecoderAnalogAudioStop();


//回放 播放混音要支持 wav 和 MP3 两种格式，下面是播放wav的接口

//功能说明: 设置混音参数
//参数说明:
//  nSample_rate: 采样率
//  nBits_per_sample: 每个采样的位数
//  nChannels:  声道数
//返回值: 0：失败  1：成功
int TH_MpegDecoderPlayWavSetParams(int nSample_rate,int nBits_per_sample, int nChannels);

//功能说明: 设置混音音量
//参数说明:
//  nVol: 混音音量
//返回值: 0：失败  1：成功
int TH_MpegDecoderPlayWavSetVolume(int nVol);


//功能说明: 开始混音
//参数说明:
//返回值: 0：失败  1：成功
int TH_MpegDecoderPlayWavStart();

//功能说明: 获得混音的缓冲区指针和缓冲区长度
//参数说明:
//  buffer: 混音数据缓冲区指针
//  p:  混音数据缓冲区大小
//返回值: 0：失败  1：成功
int TH_MpegDecoderPlayWavGetBuf(char **buffer, int *p);

//功能说明: 播放混音的缓冲区数据
//参数说明:
//  nLen: 混音数据长度
//返回值: 0：失败  1：成功
int TH_MpegDecoderPlayWavPushBuf(int nLen);//通知混音设备所填充的缓冲区长度是多少

//功能说明: 停止混音
//参数说明:
//返回值: 0：失败  1：成功
int TH_MpegDecoderPlayWavStop();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////关于3D纹理更新的接口//////////////////////////////
/*完全替代glTexImage2D的函数，前面的参数和glTexImage2D一样，
surface：传入一个surface，生成的纹理将复制一份该surface的图像数据
FunCallback：传入glTexImage2D即可
*/
typedef void (*glTexImage2D_Callback)(unsigned int target, int level, int internalformat, int width, int height, int border, unsigned int  format, unsigned int type, void* datas);
void private_glTexImage2D (unsigned int target, int level, int internalformat, int width, int height, int border, unsigned int  format, unsigned int type, int surface, glTexImage2D_Callback FunCallback);


typedef void (*glTexSubImage2D_Callback)(unsigned int target, int level, int xoffset, int yoffset, int width, int height, unsigned int format, unsigned int type, void *pixels);
void private_glTexSubImage2D (unsigned int target, int level, int xoffset, int yoffset, int width, int height, int border, unsigned int  format, unsigned int type, int surface, glTexSubImage2D_Callback FunCallback);

#ifdef __cplusplus
}
#endif

#endif 


