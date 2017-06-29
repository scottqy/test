

#include "UC_SDK_API.h"
#include "rbd.h"
#include "rbddef.h"
#include "hwinfo.h"

#include "libcloud.h"

void print_useage()
{
	printf("useage:[picfile]\n");
}
// 创建一个表面
static PSURFACE create_surface(int w,int h)
{
	PSURFACE surface = NULL;
	if(w > 0 && h >0)
	{
		surface = (PSURFACE) malloc(sizeof(SURFACE));
		if(surface)
		{
			int n = TH_CreateSurface(w,h);
			if(n)
			{
				// bsurface_settings settings;
				bsurface_memory memory;
				// TH_GetSurfaceInfo(n,&settings);
				TH_GetSurfaceMemory(n,&memory);
				surface->mem_handle = n;
				surface->width = w;
				surface->height = h;
				surface->color_mode = 0;
				surface->size = w * h * 4;
				surface->vir_addr = (ULONG) memory.buffer;

				// 由于原SURFACE中不含pitch，所以此处用phy_addr来表示pitch
				surface->phy_addr = memory.pitch;


				/* 
					// 填充颜色
				sRECT rect;
				rect.x=0;
				rect.y=0;
				rect.width = w;
				rect.height = h;
				TH_SurfaceFill(n,&rect,0xFF8000FF);
				TH_SetGraphicsFramebuffer(DISPLAY_VGA,n);
				*/ 

				// 申请成功返回创建好的SURFACE结构

/*				printf("=== CreateSurface:%p\n",surface);
				printf("psurface->alloc_size=%d\npsurface->width=%d\npsurface->height=%d\n"
					",psurface->pitch=%d\n,psurface->size=%d\npsurface->vir_addr=%p\npsurface->phy_addr=%p\n\n",
					surface->alloc_size,
					surface->width,surface->height,
					surface->pitch,surface->size,
					surface->vir_addr,surface->phy_addr);
*/
				return surface;
			}
			else
			{
				// 申请失败
				free(surface);
				return NULL;
			}
		}
		else
			return NULL;
	}
	return NULL;
}

// 释放一个SURFACE
static int destroy_surface(PSURFACE psurface)
{
	if(psurface)
	{
		if(psurface->mem_handle)
		{
			TH_DestorySurface(psurface->mem_handle);
/*			printf("===  To destroy surface: %p\n",psurface);
			printf("psurface->alloc_size=%d\npsurface->width=%d\npsurface->height=%d\n"
				",psurface->pitch=%d\n,psurface->size=%d\npsurface->vir_addr=%p\npsurface->phy_addr=%p\n\n",
				psurface->alloc_size,
				psurface->width,psurface->height,
				psurface->pitch,psurface->size,
				psurface->vir_addr,psurface->phy_addr);
*/
			free(psurface);
			return 1;
		}
	}
	return 0;
}

int main(int argc, char ** argv)
{
	if(argc != 2) {print_useage();exit(0);}
	TH_KTV_sys_init();
	Image img;
	string filename = argv[1];
	img.SetFile(filename);
	img.CreateMemSurface();
	// 分配显存成功后加载图片
	if(!img.LoadIntoSurface()) {
		printf("image load failed!\n");
		print_useage();
		exit(0);
	} else {
		PSURFACE vga_sur = create_surface(1280,720);
		TH_SetGraphicsFramebuffer(0, vga_sur->mem_handle);
		//rbdFillRect(vga_sur,NULL,0xFFFFFFFF);
		//rbdBitBlt(vga_sur,NULL,BM_DEST_P2_SRC_P1,img.GetSurface(),NULL,0x00);
		TH_SurfaceFill(vga_sur->mem_handle, &sRECT{0,0,1280,720}, 0x0);
		TH_SurfaceCopy(vga_sur->mem_handle, NULL,img.GetSurface()->mem_handle, NULL);
		
		if(destroy_surface(vga_sur) == 1)
			printf("end & sccess\n");
		else 
			printf("destroy error\n");
	}
}
