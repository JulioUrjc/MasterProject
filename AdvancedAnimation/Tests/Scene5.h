#ifndef SCENE5_H
#define SCENE5_H

#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

class Scene5 : public Test {
	public:
		Scene5(){
		{
			porcentaje3 = 20;
			porcentaje = 50;
			porcentaje2 = 100;
			porcentaje4 = 70;

			m_position = 0;
			m_position2 = 0;
			m_position3 = 0;
			// Initialize the particle emitter1.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter.SetParticleSystem(m_particleSystem);
				m_emitter.SetPosition(b2Vec2(250.0f, 550.0f));
				m_emitter.SetVelocity(b2Vec2(-0.1f, 0.0f));
				m_emitter.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter.SetEmitRate(400.0f);
				m_emitter.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter2.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter2.SetParticleSystem(m_particleSystem);
				m_emitter2.SetPosition(b2Vec2(2.5f, 400.0f));
				m_emitter2.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter2.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter2.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter2.SetEmitRate(400.0f);
				m_emitter2.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter3.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter3.SetParticleSystem(m_particleSystem);
				m_emitter3.SetPosition(b2Vec2(-300.5f, 350.0f));
				m_emitter3.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter3.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter3.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter3.SetEmitRate(400.0f);
				m_emitter3.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter4.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter4.SetParticleSystem(m_particleSystem);
				m_emitter4.SetPosition(b2Vec2(350.5f, 200.0f));
				m_emitter4.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter4.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter4.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter4.SetEmitRate(400.0f);
				m_emitter4.SetParticleFlags(TestMain::GetParticleParameterValue());
			}
			Zoom = 25.0f;
			m_world->SetGravity(b2Vec2(0, -10));

			// Configure particle system parameters.
			m_particleSystem->SetRadius(8.065f);
			m_particleSystem->SetMaxParticleCount(100000);
			m_particleSystem->SetDestructionByAge(true);

			TestMain::GetFilesNames(geomFile_, neuronFile_);
				
			std::vector<b2Vec2> geomRead;
			if (geomFile_ != ""){
				FILE *archivo;
				archivo = fopen(geomFile_.c_str(), "r");
				float x, y;

				fscanf(archivo, "%f %f", &x, &y);
				geomRead.push_back(b2Vec2(x, y));
			}

				b2BodyDef bd;
				b2Body* ground = m_world->CreateBody(&bd);
				b2ChainShape bodyGeom;						

				//for (int i = 0; i < geomRead.size(); ++i){
				//	//geomRead[i] -= b2Vec2(600.0f, 800.0f);
				//	//geomRead[i] = geomRead[i].Skew4();
				//}

				//bodyBrain.CreateLoop(brain, 93);
				bodyGeom.CreateLoop(&geomRead[0], geomRead.size());

				b2FixtureDef def;
				def.shape = &bodyGeom;
				ground->CreateFixture(&def);

				// Create brodman
				{
					b2BodyDef bd;
					b2Body* ground = m_world->CreateBody(&bd);
					b2ChainShape bodybrodman;
					b2Vec2 brodman[4] = {
						b2Vec2(286.0f, 538.0f), b2Vec2(586.0f, 115.0f),
						b2Vec2(819.0f, 528.0f), b2Vec2(1095.0f, 450.0f)
					};

					for (int i = 0; i < 4; ++i){
						brodman[i] -= b2Vec2(600.0f, 800.0f);
						brodman[i] = brodman[i].Skew4();
					}

					bodybrodman.CreateChain(brodman, 4);
					b2FixtureDef def;
					def.shape = &bodybrodman;
					ground->CreateFixture(&def);
				}
			}


			// Don't restart the test when changing particle types.
			TestMain::SetRestartOnParticleParameterChange(false);
			// Limit the set of particle types.
			TestMain::SetParticleParameters(k_paramDef, k_paramDefCount);
			// Create the particles.
			//ResetParticles();
		}		

		void setFiles(const string geomFile, const string neuronFile){ geomFile_ = geomFile; neuronFile_ = neuronFile; }
		
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