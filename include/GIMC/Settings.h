// Settings.h

#pragma once

namespace GIMC
{
	struct ParticleSystemSettings
	{
		unsigned definitionLimit; ///< Maximum particle types allowed
		unsigned particleLimit; ///< Maximum particles per type allowed
		unsigned emitterLimitPerDefinition; ///< Maximum emitters types allowed

		ParticleSystemSettings()
			: definitionLimit(100)
			, particleLimit(10000)
			, emitterLimitPerDefinition(100)
		{}
	};
};