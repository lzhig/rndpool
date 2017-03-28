//生成服务插件时需要定义该宏，使用不要定义
#ifdef SERVICE_EXPORTS
#	ifdef _MSC_VER
#		define SERVICE_API	__declspec( dllexport )
#	else //gcc 4.x+
#		define SERVICE_API	__attribute__((__visibility__("default")))
#	endif //vc + gcc
#else
#	define SERVICE_API
#endif //SERVICE_EXPORTS


extern "C"
{
    SERVICE_API bool init_pool()
    {
		return true;
    }

	SERVICE_API void release_pool()
	{

	}
}