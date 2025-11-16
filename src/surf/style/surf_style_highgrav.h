#include "surf_style.h"
#include "version_gen.h"

#define STYLE_NAME       "HighGrav"
#define STYLE_NAME_SHORT "HG"

class SurfHighGravStylePlugin : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	bool Pause(char *error, size_t maxlen);
	bool Unpause(char *error, size_t maxlen);

public:
	const char *GetAuthor()
	{
		return PLUGIN_AUTHOR;
	}

	const char *GetName()
	{
		return "CS2Surf-Style-HighGrav";
	}

	const char *GetDescription()
	{
		return "High Grav style plugin for CS2Surf";
	}

	const char *GetURL()
	{
		return PLUGIN_URL;
	}

	const char *GetLicense()
	{
		return PLUGIN_LICENSE;
	}

	const char *GetVersion()
	{
		return PLUGIN_FULL_VERSION;
	}

	const char *GetDate()
	{
		return __DATE__;
	}

	const char *GetLogTag()
	{
		return PLUGIN_LOGTAG;
	}
};

class SurfHighGravStyleService : public SurfStyleService
{
	using SurfStyleService::SurfStyleService;

public:
	virtual const char *GetStyleName() override
	{
		return "HighGrav";
	}

	virtual const char *GetStyleShortName() override
	{
		return "HG";
	}

	virtual const CVValue_t *GetTweakedConvarValue(const char *name) override;
	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void OnProcessMovement() override;
};
