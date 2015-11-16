// Settings.h

#pragma once

namespace GIMC
{
	static const unsigned MAX_DEFINITIONS = 100;
	static const unsigned MAX_PARTICLES = 10000;
	static const unsigned MAX_EMITTERS = 100;
	static const unsigned MAX_SPAWNCOUNT = 50;

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