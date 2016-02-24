#ifndef SCENE5_H
#define SCENE5_H

#include <iostream>
#include <time.h>
using namespace std;

class Scene5 : public Test {
	public:
		Scene5(){		
			
		};

		virtual void Step(Settings* settings){
			Test::Step(settings);
			// Number of particles below (bottom) the barrier.
			// int32 top = 0;
		
			

			m_debugDraw.DrawString(700, 60, "Time: %d", 0.5);
			//m_debugDraw.DrawString(700, 75, "Barrier Pos: %f", m_position);
			//m_debugDraw.DrawString(700, 90, "Num Particles2: %i", bottom2);
			//m_debugDraw.DrawString(700, 105, "Barrier Pos2: %f", m_position2);
			//m_debugDraw.DrawString(700, 120, "Num Particles3: %i", bottom3);
			//m_debugDraw.DrawString(700, 135, "Barrier Pos3: %f", m_position3);
		}

		// Determine whether a point is in the container.
		bool InContainer(const b2Vec2& p) const{ return p.x >= -k_containerHalfWidth && p.x <= k_containerHalfWidth && p.y >= 0.0f && p.y <= k_containerHalfHeight * 2.0f; }
		float32 GetDefaultViewZoom() const{ return Zoom; }
		static Test* Create(){ return new Scene5; }

	private:
		float32 m_particleColorOffset;
		RadialEmitter m_emitter;
		RadialEmitter m_emitter2;
		RadialEmitter m_emitter3;
		RadialEmitter m_emitter4;
		int32 porcentaje;
		int32 porcentaje2;
		int32 porcentaje3;
		int32 porcentaje4;

		float32 m_position;
		float32 m_position2;
		float32 m_position3;

		b2Body* m_barrierBody;
		b2Body* m_barrierBody2;
		b2Body* m_barrierBody3;
		b2ParticleGroup* m_particleGroup;

	private:
		static const int32 k_maxParticleCount;
		static const ParticleParameter::Value k_paramValues[];
		static const ParticleParameter::Definition k_paramDef[];
		static const uint32 k_paramDefCount;

		static const float32 k_containerWidth;
		static const float32 k_containerHeight;
		static const float32 k_containerHalfWidth;
		static const float32 k_containerHalfHeight;
		static const float32 k_barrierHeight;
		static const float32 k_barrierMovementIncrement;

		static const float32 k_faucetLength;
		static const float32 k_faucetWidth;
		static const float32 k_faucetHeight;
};

const float32 Scene5::k_faucetLength = 2.0f;
const float32 Scene5::k_faucetWidth = 0.1f;
const float32 Scene5::k_faucetHeight = 15.0f;

const float32 Scene5::k_containerWidth = 2.0f;
const float32 Scene5::k_containerHeight = 5.0f;

const float32 Scene5::k_containerHalfWidth = Scene5::k_containerWidth / 2.0f;
const float32 Scene5::k_containerHalfHeight = Scene5::k_containerHeight / 2.0f;
const float32 Scene5::k_barrierHeight = Scene5::k_containerHalfHeight / 100.0f;
const float32 Scene5::k_barrierMovementIncrement = Scene5::k_containerHalfHeight * 0.1f;


const int32 Scene5::k_maxParticleCount = 10000;
const ParticleParameter::Value Scene5::k_paramValues[] = {
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions, "water" },
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions | ParticleParameter::OptionStrictContacts, "water (strict)" },
	{ b2_viscousParticle, ParticleParameter::k_DefaultOptions, "viscous" },
	{ b2_powderParticle, ParticleParameter::k_DefaultOptions, "powder" },
	{ b2_tensileParticle, ParticleParameter::k_DefaultOptions, "tensile" },
	{ b2_colorMixingParticle, ParticleParameter::k_DefaultOptions, "color mixing" },
	{ b2_staticPressureParticle, ParticleParameter::k_DefaultOptions, "static pressure" }, };

const ParticleParameter::Definition Scene5::k_paramDef[] = { { Scene5::k_paramValues, B2_ARRAY_SIZE(Scene5::k_paramValues) }, };
const uint32 Scene5::k_paramDefCount = B2_ARRAY_SIZE(Scene5::k_paramDef);

#endif