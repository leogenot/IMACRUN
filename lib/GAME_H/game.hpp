#ifndef GAME_H
#define GAME_H
#include "gamemap.hpp"
#include "sceneLight.hpp"
#include "player.hpp"
#include "eyeCamera.hpp"
#include "trackballCamera.hpp"
#include "skybox.hpp"

class Game
{
private: 
    Player m_player;
    GameMap m_gameMap;
    Skybox m_skybox;
    TrackballCamera m_trackballCamera;
    eyeCamera       m_eyeCamera;

public:
    bool paused;
    bool fixedCamera;

    Game(SceneLight sceneLight): m_gameMap(sceneLight), m_player(&m_eyeCamera), paused(true), fixedCamera(false) {};
    void renderGame(float window_width, float window_height, Model objModel) 
    {
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = m_player.getCamera()->GetViewMatrix(m_player.getPos());
        glm::mat4 projection = glm::perspective(m_eyeCamera.Zoom, window_width / window_height, 0.1f, 100.0f);

        // draw player
        if (m_player.getCamera()->getCameraType() == 0) //no drawing with eye camera
            m_player.draw(view, projection, model, objModel);

        // draw gameMap
        m_gameMap.drawGameMap(view, projection, model, m_player.getCamera()->getPos());
    
        // draw skybox
        m_skybox.draw(view, projection, model, m_player.getCamera()->GetViewMatrix(m_player.getPos()));
    };

    // getter
    GameMap* getGameMap() {return &m_gameMap;};
    Player* getPlayer() {return &m_player;};

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
        m_gameMap.loadGameMap(pgmFile); // generate gamemap whith file
        m_gameMap.initObstacles(nbObstacles);
        m_gameMap.initLights(nbLights);
        m_skybox.initSkybox();
    }
    void PauseGame() {paused = true;};
    void ResumeGame() {paused = false;};
    void EndGame();
    void ResetGame() 
    {
        paused = !paused;
        m_player.ResetPlayer();
    };
    void LoadGame(); //load from file data
    void SaveGame();
};

#endif