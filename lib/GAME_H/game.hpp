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
    Player          m_player;
    Enemy           m_enemy;
    GameMap         m_gameMap;
    Skybox          m_skybox;
    TrackballCamera m_trackballCamera;
    eyeCamera       m_eyeCamera;
    vector<Player>  m_player_list;

    // assets
    std::vector<Model>  m_objModels;
    std::vector<Shader> m_shaders;

    Game() {};
    Game(SceneLight sceneLight)
        : m_gameMap(sceneLight), m_player(&m_eyeCamera), paused(true), fixedCamera(false){};


protected : static Game* m_instance;

public:
    bool paused;
    bool fixedCamera;

    /* This is the static method that controls the access to the singleton
    instance. On the first run, it creates a singleton object and places it
    into the static field. On subsequent runs, it returns the client existing
    object stored in the static field.*/
    static Game* instance(SceneLight scenelight);

    void renderGame(float window_width, float window_height);
    void setShader(Shader shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, int renderRadius);

    // getter
    GameMap*        getGameMap() { return &m_gameMap; };
    Player*         getPlayer() { return &m_player; };
    vector<Player>* getPlayerList() { return &m_player_list; };
    Enemy*          getEnemy() { return &m_enemy; };

    void switchCamera();

    // game menu manager
    void InitGame(string pgmFile, int nbObstacles, int nbLights);
    bool LoseGame();
    void ResetGame(int nbObstacles, int nbLights);

    void LoadGame(){}; //load from file data à supprimer ?
    void SaveGame(){}; // à supprimer ?

    void SavePlayerData();
    bool exists(const json& j, const std::string& key);
    void ShowPlayersData();
    void LoadPlayerData();
};

#endif