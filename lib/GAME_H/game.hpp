#ifndef GAME_H
#define GAME_H
#include <vector>
#include "enemy.hpp"
#include "eyeCamera.hpp"
#include "gamemap.hpp"
#include "json.hpp"
#include "player.hpp"
#include "sceneLight.hpp"
#include "skybox.hpp"
#include "trackballCamera.hpp"
using json = nlohmann::json;

class Game {

private: 
    Player              m_player;
    Enemy               m_enemy;
    GameMap             m_gameMap;
    Skybox              m_skybox;
    TrackballCamera     m_trackballCamera;
    eyeCamera           m_eyeCamera;
    vector<Player>      m_player_list;

    // assets
    std::vector<Model>  m_objModels;
    std::vector<Shader> m_shaders;

public:
    bool paused;
    bool fixedCamera;

    Game(SceneLight sceneLight)
    : m_gameMap(sceneLight), m_player(&m_eyeCamera), paused(true), fixedCamera(false) {};

    void renderGame(float window_width, float window_height)
    {
        int renderRadius = 8;
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = m_player.getCamera()->GetViewMatrix(m_player.getPos(), m_player.down);
        glm::mat4 projection = glm::perspective(m_eyeCamera.Zoom, window_width / window_height, 0.1f, 100.0f);

        // draw player
        if (m_player.getCamera()->getCameraType() == 0) //no drawing with eye camera
        {
            setShader(m_player.getShader(), m_player.getModel(), view, projection, renderRadius);
            m_player.getObjModel().DrawModel(m_player.getShader());
        }

        //draw enemy
        setShader(m_enemy.getShader(), m_enemy.getModel(), view, projection, renderRadius);
        m_enemy.getObjModel().DrawModel(m_enemy.getShader());

        // draw gameMap
        m_gameMap.drawGameMap(view, projection, model, m_player.getCamera()->getPos(), m_player.getPos(), 8);

        // draw skybox
        m_skybox.draw(view, projection, model, m_player.getCamera()->GetViewMatrix(m_player.getPos(), m_player.down));
    };

    // getter
    GameMap*        getGameMap() { return &m_gameMap; };
    Player*         getPlayer() { return &m_player; };
    vector<Player>* getPlayerList() { return &m_player_list; };
    Enemy*          getEnemy() { return &m_enemy; };

    void setShader(Shader shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, int renderRadius)
    {
        shader.use();

        // point lights
        int i = 0;
        std::vector<Light*> lights = m_gameMap.getLights();
        for (auto it = lights.begin(); it != lights.end(); it++)
        {
            if ((*it)->getPos().x < m_player.getPos().x + renderRadius && (*it)->getPos().x > m_player.getPos().x - renderRadius && (*it)->getPos().y < m_player.getPos().y + renderRadius && (*it)->getPos().y > m_player.getPos().y - renderRadius)
            {
                std::string uniformNamePosition = "pointLights[" + std::to_string(i) + "].position";
                std::string uniformNameColor = "pointLights[" + std::to_string(i) + "].color";
                
                shader.setVec3(uniformNamePosition, glm::vec3((*it)->getPos()));
                shader.setVec3(uniformNameColor, (*it)->getColor());
                i++;
            }
        }

        // light
        shader.setVec3("dirLight", m_gameMap.getSceneLight().getDirection());
        shader.setVec3("lightColor", m_gameMap.getSceneLight().getColor());
        shader.setVec3("viewPos", m_player.getCamera()->getPos());

        // material
        shader.setFloat("material.shininess", 32.0f);

        // matrix
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
    };

    void switchCamera()
    {
        if (m_player.getCamera()->getCameraType() == 0) {
            m_player.setCamera(&m_eyeCamera);
        }
        else {
            m_player.setCamera(&m_trackballCamera);
        }
    };

    // game menu manager
    void InitGame(string pgmFile, int nbObstacles, int nbLights)
    {
        //initialize tab of shader
        Shader lightShader("GAME/shaders/lightSource.vs", "GAME/shaders/lightSource.fs");
        Shader objShader("GAME/shaders/model_loading.vs", "GAME/shaders/model_loading.fs");
        m_shaders.push_back(lightShader);
        m_shaders.push_back(objShader);

        //initialize tab of 3DModel
        Model lightModel("assets/models/lightning_bolt.obj");
        Model playerModel("assets/models/flash.obj");
        Model enemyModel("assets/models/franck.obj");
        m_objModels.push_back(lightModel);
        m_objModels.push_back(playerModel);
        m_objModels.push_back(enemyModel);

        m_enemy.initEnemy(&m_shaders[1], &m_objModels[2]);
        m_gameMap.loadGameMap(pgmFile); // generate gamemap whith file
        m_player.initPlayer(&m_shaders[1], &m_objModels[1]);
        m_gameMap.initObstacles(nbObstacles);
        m_gameMap.initLights(nbLights, &m_shaders[0], &m_objModels[0]);
        m_skybox.initSkybox();
    };
    bool LoseGame()
    {
        if (getPlayer()->getLife() == 0 || getPlayer()->getCollision(FORWARD, getGameMap()) == true) {
            paused = !paused;
            return true;
        }
        return false;
    };
    void ResetGame(int nbObstacles, int nbLights)
    {
        m_eyeCamera.resetEyeCamera();
        m_player.resetPlayer();
        m_player.setCamera(&m_eyeCamera);
        m_enemy.resetEnemy();
        m_gameMap.resetGameMap(nbObstacles, nbLights, &m_shaders[0], &m_objModels[0]);
        paused = !paused;
    };
    void LoadGame(){}; //load from file data
    void SaveGame(){};

    void SavePlayerData()
    {
        // read a JSON file
        std::ifstream readingFile(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur lecture fichier
        if (readingFile.is_open()) {
            json jscores;
            readingFile >> jscores;
            readingFile.close();
            if (jscores.contains("BOB")) {
                cout << "it is found" << endl;
            }
            else {
                cout << "not found " << endl;
            }

            //add new score
            if (exists(jscores, m_player.getUsername())) {
                cout << "User already exist" << endl;
                json jscores_update = {{m_player.getUsername(), m_player.getScore()}};

                // add all keys from object2 to object1
                jscores.update(jscores_update);
            }

            else
                jscores += {m_player.getUsername(), m_player.getScore()};

            // write prettified JSON to another file
            std::ofstream writtingFile(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur
            if (writtingFile.is_open()) {
                writtingFile << jscores << std::endl;
                writtingFile.close();
            }
            else
                cout << "Unable to open file WRITE";
        }
        else
            cout << "Unable to open file READ";
    };

    bool exists(const json& j, const std::string& key)
    {
        return j.find(key) != j.end();
    };

    void ShowPlayersData()
    {
        // read a JSON file
        std::ifstream i(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur lecture fichier
        json          json;
        i >> json;
        std::cout << "Scores : " << std::endl;
        // iterate the array

        for (auto it = json.begin(); it != json.end(); ++it) {
            std::cout << it.key() << " "
                      << ": " << it.value();

            std::cout << std::endl;
        }
    };

    void LoadPlayerData()
    {
        // read a JSON file
        std::ifstream i(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur lecture fichier
        json          json;
        i >> json;
        for (auto it = json.begin(); it != json.end(); ++it) {
            std::cout << it.key() << " "
                      << ": " << it.value() << std::endl;
        }
    };
};

#endif