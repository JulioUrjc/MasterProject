#ifndef SCENE8_H
#define SCENE8_H

#include <iostream>
#include <time.h>
#include <vector>
#include <map>

using namespace std;

class Scene8 : public Test {
public:
	Scene8(){
		m_world->SetGravity(b2Vec2(0, -10));

		TestMain::GetFilesNames(geomFile_, neuronFile_);

		vector<vector<b2Vec2>> geomRead;
		vector<vector<b2Vec2>> particiones;
		vector<b2Vec2> neurons;
		vector<unsigned int> numGvertex;
		vector<unsigned int> numPvertex;
		vector<int> loops;
		vector<float>  times;

		// { Read Geom and Partition File}
		for (unsigned int numfile = 0; numfile < geomFile_.size(); ++numfile){
			FILE *archivo;
			archivo = fopen(geomFile_.at(numfile).c_str(), "r");

			float x, y;
			int maxParticles;
			unsigned int r, g, b, a;
			unsigned int shapes, partitions, numVertex, emiters, loop;

			fscanf(archivo, "%u", &shapes);
			for (unsigned int i = 0; i < shapes; ++i){
				fscanf(archivo, "%u", &numVertex);
				numGvertex.push_back(numVertex);
			}
			fscanf(archivo, "%u", &partitions);
			for (unsigned int i = 0; i < partitions; ++i){
				fscanf(archivo, "%u", &numVertex);
				numPvertex.push_back(numVertex);
			}
			fscanf(archivo, "%u", &emiters);
			for (unsigned int i = 0; i < shapes; ++i){
				fscanf(archivo, "%d", &loop);
				loops.push_back(loop);
			}

			// Geom
			for (unsigned int shape = 0; shape < numGvertex.size(); ++shape){
				geomRead.push_back(vector<b2Vec2>());
				for (unsigned int cont = 0; cont < numGvertex.at(shape); ++cont)
				{
					fscanf(archivo, "%f %f", &x, &y);
					maxX = max(x, maxX);
					minX = min(x, minX);
					maxY = max(y, maxY);
					minY = min(y, minY);
					geomRead.at(geomRead.size() - 1).push_back(b2Vec2(x, y));
				}
			}
			Zoom = max((maxX - minX), (maxY - minY)) / 2;

			// Partition
			for (unsigned int part = 0; part < numPvertex.size(); ++part){
				particiones.push_back(vector<b2Vec2>());
				for (unsigned int cont = 0; cont < numPvertex.at(part); ++cont){
					fscanf(archivo, "%f %f", &x, &y);
					particiones.at(particiones.size() - 1).push_back(b2Vec2(x, y));
				}
			}

			//Emisores
			if (emiters > 0){
				fscanf(archivo, "%f %d", &x, &maxParticles);

				m_particleSystem->SetRadius(x);
				m_particleSystem->SetMaxParticleCount(maxParticles);
				m_particleSystem->SetDestructionByAge(true);

				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;

				for (unsigned int cont = 0; cont < emiters; ++cont){
					RadialEmitter rEmit;

					rEmit.SetParticleSystem(m_particleSystem);
					fscanf(archivo, "%f %f", &x, &y);
					rEmit.SetPosition(b2Vec2(x, y));
					fscanf(archivo, "%f %f", &x, &y);
					rEmit.SetVelocity(b2Vec2(x, y));
					rEmit.SetSize(b2Vec2(0.0f, faucetLength));
					fscanf(archivo, "%u %u %u %u", &r, &g, &b, &a);
					rEmit.SetColor(b2ParticleColor((uint8)r, (uint8)g, (uint8)b, (uint8)a));
					fscanf(archivo, "%f", &x);
					rEmit.SetEmitRate(x);
					rEmit.SetParticleFlags(TestMain::GetParticleParameterValue());

					m_emitters.push_back(rEmit);
				}
			}
			fclose(archivo);

			// Geom Scene
			for (unsigned int shape = 0; shape < geomRead.size(); ++shape){
				try {
					if (geomRead.at(shape).size() < 2) throw (int)geomRead.at(shape).size();
					// Vertex Geom
					b2BodyDef bd;
					b2Body* ground = m_world->CreateBody(&bd);
					b2ChainShape bodyGeom;

					if (loops.at(shape)){
						bodyGeom.CreateLoop(&(geomRead.at(shape))[0], geomRead.at(shape).size());
					}
					else{
						bodyGeom.CreateChain(&(geomRead.at(shape))[0], geomRead.at(shape).size());
					}

					b2FixtureDef defB;
					defB.shape = &bodyGeom;
					ground->CreateFixture(&defB);
				}
				catch (int error) {
					cerr << "shape: " << shape << " " << error << " puntos. Debe contener al menos 2 puntos" << endl;
				}
			}

			// Partitions Scene
			for (unsigned int part = 0; part < particiones.size(); ++part){
				try {
					if (particiones.at(part).size() < 2) throw (int)particiones.at(part).size();
					// Partition Vertex
					b2BodyDef bdP;
					b2Body* groundP = m_world->CreateBody(&bdP);
					b2ChainShape partitionGeom;

					partitionGeom.CreateChain(&(particiones.at(part))[0], particiones.at(part).size());

					b2FixtureDef defP;
					defP.shape = &partitionGeom;
					groundP->CreateFixture(&defP);
				}
				catch (int error) {
					cerr << "part: " << part << " " << error << " puntos. Debe contener al menos 2 puntos" << endl;
				}
			}
		}

		// { Read Neuron File}
		if (neuronFile_ != ""){
			FILE *archivoNeuron;
			archivoNeuron = fopen(neuronFile_.c_str(), "r");

			string begin = "";
			char line[200];
			
			while (begin != "// TOTAL NUMBER OF NEURONS\n")
			{
				if (fgets(line, 100, archivoNeuron) == NULL) break;
				begin = line;
			}

			unsigned int numNeurons = 0;
			fscanf(archivoNeuron, "%d", &numNeurons);

			while (begin != "//DEFINE THE LAYERS\n")
			{
				if (fgets(line, 100, archivoNeuron) == NULL) break;
				begin = line;
			}

			fgets(line, 100, archivoNeuron); // Blank space
			while (begin != "\n")
			{
				if (fgets(line, 100, archivoNeuron) == NULL) break;
				begin = line;
				if (begin[0] == begin[1] && begin.substr(0, 1) == "/"){	
					layers.push_back(begin.substr(3, begin.size()));
				}
			}
			cout <<"Layers Num: "<< layers.size() << endl;

			for (unsigned i = 0; i < layers.size(); ++i)
				cout << layers.at(i).c_str();

			while (begin != "// Generated by GenerateConnectionsMFGR\n")
			{
				if (fgets(line, 100, archivoNeuron)== NULL) break;
				begin = line;
			}

			unsigned int numConnections = 0;
			fscanf(archivoNeuron, "%s %d %s", line, &numConnections, line);

			int x1, x2, x3, x4, x5, x6, neuron1, neuron2;
			float y1, y2;
			conexion = vector<map<int, int>>(numNeurons);
			for (unsigned int cont = 0; cont < numConnections; ++cont){
				fscanf(archivoNeuron, "%d %d %d %d %d %f %d %d %f %d",
					&neuron1, &x1, &neuron2, &x2, &x3, &y1, &x4, &x5, &y2, &x6);

				if (conexion.at(neuron1).count(neuron2) > 0){
					conexion.at(neuron1)[neuron2] += 1;
				}
				else{
					conexion.at(neuron1)[neuron2]= 1; // neuron and count
				}
					
				maxConexions = (conexion.at(neuron1).size() > conexion.at(maxConexions).size()) ? neuron1 : maxConexions;
				neurons.push_back(b2Vec2((float)neuron1, (float)neuron2));
			}

			fclose(archivoNeuron);
		}
		// Don't restart the test when changing particle types.
		TestMain::SetRestartOnParticleParameterChange(false);
		// Limit the set of particle types.
		TestMain::SetParticleParameters(k_paramDef, k_paramDefCount);
		// Create the particles.
		//ResetParticles();			
		time(&initHour);
		//std::srand(time(NULL));
	}

	void setFiles(const vector<string> geomFile, const string neuronFile){ geomFile_ = geomFile; neuronFile_ = neuronFile; }

	b2ParticleColor colorPorcentaje(int p){
		int intervalo = (p>0) + (p>20) + (p>40) + (p>60) + (p>80);
		switch (intervalo){
		case 0: return b2ParticleColor(0, 0, 0, 0); // Transparente
		case 1:	return b2ParticleColor(0, 255, 0, 255); // green
		case 2: return b2ParticleColor(255, 0, 255, 255);
		case 3: return b2ParticleColor(0, 0, 255, 255); // blue
		case 4: return b2ParticleColor(255, 255, 0, 255);
		case 5: return b2ParticleColor(255, 0, 0, 255); //red
		default:
			return b2ParticleColor(255, 255, 255, 255);
			break;
		}
	}

	bool colorAction(int emiter){
		b2ParticleColor color;
		int colorRand = rand() % 6;

		switch (colorRand){
		case 0: color = b2ParticleColor(150, 100, 50, 200); break;// Grisaceo
		case 1: color = b2ParticleColor(0, 255, 0, 255); break;   // green
		case 2: color = b2ParticleColor(255, 0, 255, 255); break;
		case 3: color = b2ParticleColor(0, 0, 255, 255); break;   // blue
		case 4: color = b2ParticleColor(255, 255, 0, 255); break;
		case 5: color = b2ParticleColor(255, 0, 0, 255); break;   //red
		default:
			color = b2ParticleColor(255, 255, 255, 255); break;
		}
		m_emitters.at(emiter).SetColor(color);
		return true;
	}

	bool radiusAction(float ra){
		m_particleSystem->SetRadius(ra);
		return true;
	}

	bool emisionAction(int emisor){
		m_emitters.at(emisor).SetEmitRate(0);
		return true;
	}

	bool emisionAddAction(int emisor){
		m_emitters.at(emisor).SetEmitRate(m_emitters.at(emisor).GetEmitRate() + 1);
		return true;
	}

	bool emisionSubAction(int emisor){
		m_emitters.at(emisor).SetEmitRate(m_emitters.at(emisor).GetEmitRate() - 1);
		return true;
	}

	bool lifeAction(double seg, int totalPart){
		if ((int)seg % 2 == 0){
			m_particleSystem->SetParticleLifetime(0 % totalPart, 1.0f);
			m_particleSystem->SetParticleLifetime(1 % totalPart, 1.0f);
			m_particleSystem->SetParticleLifetime(2 % totalPart, 1.0f);
			m_particleSystem->SetParticleLifetime(3 % totalPart, 1.0f);
			m_particleSystem->SetParticleLifetime(4 % totalPart, 1.0f);
			m_particleSystem->SetParticleLifetime(5 % totalPart, 1.0f);
			m_particleSystem->SetParticleLifetime(6 % totalPart, 1.0f);
			m_particleSystem->SetParticleLifetime(7 % totalPart, 1.0f);
		}
		return true;
	}

	bool actions(int action, int emiter){

		switch (action){
		case 0: colorAction(emiter); break;
		case 1: radiusAction(m_particleSystem->GetRadius() + 0.005f); break;
		case 2: emisionAction(emiter); break;
		case 3: lifeAction(0, m_particleSystem->GetParticleCount()); break;
		case 4: break;
		default:
			cout << "Opcion no valida" << endl; break;
		}

		return true;
	}

	bool actions(double seg){
		if ((((int)seg) % 5) == 0){
			colorAction(0);
		}

		if ((((int)seg) % 5) == 0){
			radiusAction(m_particleSystem->GetRadius() + 0.005f);
		}

		if ((seg > 0) && (((int)seg) % 11) == 0){
			emisionAction(0);
		}

		if ((seg>6) && (m_particleSystem->GetParticleCount() > 0)){
			lifeAction(seg, m_particleSystem->GetParticleCount());
		}

		return true;
	}

	virtual void Step(Settings* settings){
		Test::Step(settings);

		const float32 dt = 1.0f / settings->hz;
		m_particleColorOffset += dt;

		if (m_particleColorOffset >= (float32)k_ParticleColorsCount){ m_particleColorOffset -= (float32)k_ParticleColorsCount; }

		for (unsigned int i = 0; i < m_emitters.size(); ++i){
			m_emitters.at(i).SetParticleFlags(TestMain::GetParticleParameterValue());
			m_emitters.at(i).Step(dt, NULL, 0); // Create the particles.
		}

		time_t currentTime;
		time(&currentTime);

		double seg = difftime(currentTime, initHour);

		//actions(seg);

		m_debugDraw.DrawString(700, 60, "Time { %02u : %02u }", (unsigned int)seg / 60, (unsigned int)seg % 60);
		m_debugDraw.DrawString(10, 75, "Layers Num: %02u", layers.size());
		
		int offset = 105;
		for (unsigned i = 0; i < layers.size(); ++i){
			m_debugDraw.DrawString(10, offset, layers.at(i).substr(0, layers.at(i).size()-1).c_str());
			offset += 20;
		}
	}

	virtual void Keyboard(unsigned char key){
		switch (key){
		case 'a':
			colorAction(0);
			cout << "Change emisor 1 color " << endl;
			break;
		case 's':
			if (m_emitters.size() > 1) colorAction(1);
			cout << "Change emisor 2 color" << endl;
			break;
		case 'd':
			radiusAction(m_particleSystem->GetRadius() + 0.005f);
			cout << "Increases 0.005 the radius" << endl;
			break;
		case 'f':
			radiusAction(m_particleSystem->GetRadius() - 0.005f);
			cout << "Radio decremented 0.005" << endl;
			break;
		case 'g':
			emisionAction(0);
			cout << "Stop emisor 1" << endl;
			break;
		case 'h':
			if (m_emitters.size() > 1) emisionAction(1);
			cout << "Stop emisor 2" << endl;
			break;
		case 'j':
			lifeAction(0, m_particleSystem->GetParticleCount());
			cout << "Destroy 7 last particles in 1 sg" << endl;
			break;
		case 'k':
			cout << "k option" << endl;
			break;
		case 'l':
			emisionAddAction(0);
			cout << "Add emision rate" << endl;
			break;
		case 'p':
			emisionSubAction(0);
			cout << "Sub emision rate" << endl;
			break;

		default:
			Test::Keyboard(key);
			break;
		}
	}

	// Determine whether a point is in the container.
	bool InContainer(const b2Vec2& p) const{ return p.x >= -k_containerHalfWidth && p.x <= k_containerHalfWidth && p.y >= 0.0f && p.y <= k_containerHalfHeight * 2.0f; }
	float32 GetDefaultViewZoom() const{ return (Zoom <= 1) ? pow(Zoom, 2) : sqrt(Zoom); }
	void GetCam(float &w, float &h) const{ w = (maxX + minX) / 2; h = (maxY + minY) / 2; }
	static Test* Create(){ return new Scene8; }

private:
	float32 m_particleColorOffset;
	vector<RadialEmitter> m_emitters;
	vector<string> layers;

	b2ParticleGroup* m_particleGroup;
	vector<map<int, int>> conexion;
	int maxConexions = 0;

	float maxX = -FLT_MAX, minX = FLT_MAX, maxY = -FLT_MAX, minY = FLT_MAX;

	time_t initHour;

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

const float32 Scene8::k_faucetLength = 2.0f;
const float32 Scene8::k_faucetWidth = 0.1f;
const float32 Scene8::k_faucetHeight = 15.0f;

const float32 Scene8::k_containerWidth = 2.0f;
const float32 Scene8::k_containerHeight = 5.0f;

const float32 Scene8::k_containerHalfWidth = Scene8::k_containerWidth / 2.0f;
const float32 Scene8::k_containerHalfHeight = Scene8::k_containerHeight / 2.0f;
const float32 Scene8::k_barrierHeight = Scene8::k_containerHalfHeight / 100.0f;
const float32 Scene8::k_barrierMovementIncrement = Scene8::k_containerHalfHeight * 0.1f;


const int32 Scene8::k_maxParticleCount = 10000;
const ParticleParameter::Value Scene8::k_paramValues[] = {
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions | ParticleParameter::OptionStrictContacts, "water (strict)" },
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions, "water" },
	{ b2_viscousParticle, ParticleParameter::k_DefaultOptions, "viscous" },
	{ b2_powderParticle, ParticleParameter::k_DefaultOptions, "powder" },
	{ b2_tensileParticle, ParticleParameter::k_DefaultOptions, "tensile" },
	{ b2_colorMixingParticle, ParticleParameter::k_DefaultOptions, "color mixing" },
	{ b2_staticPressureParticle, ParticleParameter::k_DefaultOptions, "static pressure" }, };

const ParticleParameter::Definition Scene8::k_paramDef[] = { { Scene8::k_paramValues, B2_ARRAY_SIZE(Scene8::k_paramValues) }, };
const uint32 Scene8::k_paramDefCount = B2_ARRAY_SIZE(Scene8::k_paramDef);

#endif