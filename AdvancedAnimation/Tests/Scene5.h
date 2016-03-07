#ifndef SCENE5_H
#define SCENE5_H

#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

class Scene5 : public Test {
	public:
		Scene5(){		
			Zoom = 25.0f;
			m_world->SetGravity(b2Vec2(0, -10));

			TestMain::GetFilesNames(geomFile_, neuronFile_);
				
			vector<b2Vec2> geomRead;
			vector<b2Vec2> particiones;

			maxX = -FLT_MAX;
			minX = FLT_MAX;
			maxY = -FLT_MAX;
			minY = FLT_MAX;

			// { Read Geom and Partition File}
			if (geomFile_ != ""){
				FILE *archivo;
				archivo = fopen(geomFile_.c_str(), "r");

				float x, y;
				unsigned int vertices;

				fscanf(archivo, "%u", &vertices);
				// Geom
				for (unsigned int cont = 0; cont < vertices; ++cont)
				{		
					fscanf(archivo, "%f %f", &x, &y);
					maxX = max(x, maxX);
					minX = min(x, minX);
					maxY = max(y, maxY);
					minY = min(y, minY);
					geomRead.push_back(b2Vec2(x, y));
				}
				// Partition
				while (fscanf(archivo, "%f %f", &x, &y) != EOF)
				{
					particiones.push_back(b2Vec2(x, y));
				}

				fclose(archivo);
			}

			try {
				// Vertex Geom
				b2BodyDef bd;
				b2Body* ground = m_world->CreateBody(&bd);
				b2ChainShape bodyGeom;

				bodyGeom.CreateLoop(&geomRead[0], geomRead.size());

				b2FixtureDef defB;
				defB.shape = &bodyGeom;
				ground->CreateFixture(&defB);

				// Partition Vertex
				b2BodyDef bdP;
				b2Body* groundP = m_world->CreateBody(&bdP);
				b2ChainShape partitionGeom;

				if (particiones.size() < 2) throw (int)particiones.size();
				partitionGeom.CreateChain(&particiones[0], particiones.size());

				b2FixtureDef defP;
				defP.shape = &partitionGeom;
				groundP->CreateFixture(&defP);
			}
			catch (int error) {
				cerr << error <<" punto. Debe contener al menos 2 puntos"<<endl;
			}				

			// { Read Neuron File}
			if (neuronFile_ != "")
			{
			
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

		float maxX, minX, maxY, minY;

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