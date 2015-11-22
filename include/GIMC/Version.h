// Version.h

#pragma once

namespace GIMC
{
	namespace internal
	{
		struct VersionInfo
		{
			unsigned major;
			unsigned minor;
			unsigned patch;
		};

		extern VersionInfo ParticleSystemVersion;
	};
};