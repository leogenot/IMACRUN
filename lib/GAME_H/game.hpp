#ifndef GAME_H
#define GAME_H
#include "eyeCamera.hpp"
#include "gamemap.hpp"
#include "json.hpp"
#include "player.hpp"
#include "sceneLight.hpp"
#include "skybox.hpp"
#include "trackballCamera.hpp"
#include "enemy.hpp"

class Game {
private:
    Player          m_player;
    Enemy           m_enemy;
    GameMap         m_gameMap;
    Skybox          m_skybox;
    TrackballCamera m_trackballCamera;
    eyeCamera       m_eyeCamera;

public:
    bool paused;
    bool fixedCamera;

    Game(SceneLight sceneLight)
        : m_gameMap(sceneLight), m_player(&m_eyeCamera), paused(true), fixedCamera(false){};
    void renderGame(float window_width, float window_height, Model player_model, Model enemy_model, Model lightning_bolt)
    {
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = m_player.getCamera()->GetViewMatrix(m_player.getPos(), m_player.down);
        glm::mat4 projection = glm::perspective(m_eyeCamera.Zoom, window_width / window_height, 0.1f, 100.0f);

        // draw player
        if (m_player.getCamera()->getCameraType() == 0) //no drawing with eye camera
            m_player.draw(view, projection, model, player_model);

        //draw enemy
        m_enemy.drawEnemy(view, projection, model, enemy_model);

        // draw gameMap
        m_gameMap.drawGameMap(view, projection, model, m_player.getCamera()->getPos(), lightning_bolt, m_player.getPos(), 8);

        // draw skybox
        m_skybox.draw(view, projection, model, m_player.getCamera()->GetViewMatrix(m_player.getPos(), m_player.down));
    };

    // getter
    GameMap* getGameMap() { return &m_gameMap; };
    Player*  getPlayer() { return &m_player; };
    Enemy*  getEnemy() { return &m_enemy; };
    

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
        m_player.initPlayer();
        m_enemy.initEnemy();
        m_gameMap.loadGameMap(pgmFile); // generate gamemap whith file
        m_gameMap.initObstacles(nbObstacles);
        m_gameMap.initLights(nbLights);
        m_skybox.initSkybox();
    }
    bool LoseGame() 
    {
        if (getPlayer()->getLife() == 0 || getPlayer()->getCollision(FORWARD, getGameMap()) == true) 
        {
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
        m_gameMap.resetObstacles(nbObstacles);
        m_gameMap.resetLights(nbLights);
        paused = !paused;
    };
    void LoadGame() {}; //load from file data
    void SaveGame() {};

    void AddScore()
    {
         using json = nlohmann::json;
        try {
            // read a JSON file
            std::ifstream readingFile(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur lecture fichier
            if (readingFile.is_open()) {
                nlohmann::json json;
                readingFile >> json;
                readingFile.close();

                //add new score
                json["scores"].push_back({{"name", m_player.getUsername()}, {"score", m_player.getScore()}});
                // write prettified JSON to another file
                std::ofstream writtingFile(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur
                if (writtingFile.is_open()) {
                    writtingFile << json << std::endl;
                    writtingFile.close();
                }
                else
                    cout << "Unable to open file WRITE";
            }
            else
                cout << "Unable to open file READ";
        }
        catch (json::parse_error& e) {
            // output exception information
            std::cout << "message: " << e.what() << '\n'
                      << "exception id: " << e.id << '\n'
                      << "byte position of error: " << e.byte << std::endl;
        } 
    };
    void ShowScores()
    {
         // read a JSON file
        std::ifstream  i(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur lecture fichier
        nlohmann::json json;
        i >> json;

        std::cout << "Scores : " << std::endl;
        // iterate the array
        for (auto it = json["scores"].begin(); it != json["scores"].end(); ++it) {
            for (auto it2 = it->begin(); it2 != it->end(); ++it2)
                std::cout << it2.value() << " ";
            std::cout << std::endl;
        } 
    }
};

#endif