#include "GAME_H/game.hpp"

Game* Game::m_instance = NULL;

/* This is the static method that controls the access to the singleton
instance. On the first run, it creates a singleton object and places it
into the static field. On subsequent runs, it returns the client existing
object stored in the static field.*/
Game* Game::instance(SceneLight scenelight)
{
    if (m_instance == NULL) 
        m_instance = new Game(scenelight);
    return m_instance;
};

void Game::renderGame(float window_width, float window_height)
{
    int       renderRadius = 8;
    glm::mat4 model        = glm::mat4(1.0f);
    glm::mat4 view         = m_player.getCamera()->GetViewMatrix(m_player.getPos(), m_player.down);
    glm::mat4 projection   = glm::perspective(m_eyeCamera.Zoom, window_width / window_height, 0.1f, 100.0f);

    if (m_player.getCamera()->getCameraType() == 0) //no drawing with eye camera
    {
        // draw player
        setShader(m_player.getShader(), m_player.getModel(), view, projection, renderRadius);
        m_player.getObjModel().DrawModel(m_player.getShader());
        //draw enemy
        setShader(m_enemy.getShader(), m_enemy.getModel(), view, projection, renderRadius);
        m_enemy.getObjModel().DrawModel(m_enemy.getShader());
    }


    // draw gameMap
    m_gameMap.drawGameMap(view, projection, model, m_player.getCamera()->getPos(), m_player.getPos(), renderRadius);

    // draw skybox
    m_skybox.draw(view, projection, model, m_player.getCamera()->GetViewMatrix(m_player.getPos(), m_player.down));
};

void Game::setShader(Shader shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, int renderRadius)
{
    shader.use();

    // point lights
    int                 i      = 0;
    std::vector<Light*> lights = m_gameMap.getLights();
    for (auto it = lights.begin(); it != lights.end(); it++) {
        if ((*it)->getPos().x < m_player.getPos().x + renderRadius && (*it)->getPos().x > m_player.getPos().x - renderRadius && (*it)->getPos().z < m_player.getPos().z + renderRadius && (*it)->getPos().z > m_player.getPos().z - renderRadius) {
            std::string uniformNamePosition = "pointLights[" + std::to_string(i) + "].position";
            std::string uniformNameColor    = "pointLights[" + std::to_string(i) + "].color";

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

void Game::switchCamera()
{
    if (m_player.getCamera()->getCameraType() == 0) {
        m_player.setCamera(&m_eyeCamera);
    }
    else {
        m_player.setCamera(&m_trackballCamera);
    }
};

void Game::InitGame(string pgmFile, int nbObstacles, int nbLights)
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

bool Game::LoseGame()
{
    if (getPlayer()->getLife() == 0 || getPlayer()->getLosingCollision(FORWARD, getGameMap()) == true || getEnemy()->collidePlayer(getPlayer()->getPos())) {
        paused = !paused;
        return true;
    }
    return false;
};

void Game::ResetGame(int nbObstacles, int nbLights)
{
    m_eyeCamera.resetEyeCamera();
    m_player.resetPlayer();
    m_player.setCamera(&m_eyeCamera);
    m_enemy.resetEnemy();
    m_gameMap.resetGameMap(nbObstacles, nbLights, &m_shaders[0], &m_objModels[0]);
    paused = !paused;
};

void Game::LoadGame(string name)
{
    char* cname;
    ifstream i(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur lecture fichier
    json json;
    i >> json;
    for (auto it = json.begin(); it != json.end(); ++it) {
        if (name == it.key())
            cname = const_cast<char*>(name.c_str());
    }

    int   player_score = json[cname].at("score");
    int   player_life  = json[cname].at("life");
    float player_pos_x = json[cname].at("position_x");
    float player_pos_y = json[cname].at("position_y");
    float player_pos_z = json[cname].at("position_z");
    float enemy_pos_x  = json[cname].at("position_enemy_x");
    float enemy_pos_y  = json[cname].at("position_enemy_y");
    float enemy_pos_z  = json[cname].at("position_enemy_z");
    float yaw          = json[cname].at("yaw");

    glm::vec3 player_pos(player_pos_x, player_pos_y, player_pos_z);
    glm::vec3 enemy_pos(enemy_pos_x, enemy_pos_y, enemy_pos_z);

    getPlayer()->setUsername(cname);
    getPlayer()->setScore(player_score);
    getPlayer()->setLife(player_life);
    getPlayer()->setPos(player_pos);
    getEnemy()->setPos(enemy_pos);
    getPlayer()->Yaw = yaw;
    getPlayer()->getCamera()->setDirection(yaw);
};



void Game::SavePlayerData()
{
    // read a JSON file
    std::ifstream readingFile(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur lecture fichier
    if (readingFile.is_open()) {
        json jscores = json::array();
        readingFile >> jscores;
        readingFile.close();

        json jscores_update = {{m_player.getUsername(), {{"score", m_player.getScore()}, {"life", m_player.getLife()}, {"position_x", m_player.getPos().x}, {"position_y", m_player.getPos().y}, {"position_z", m_player.getPos().z}, {"position_enemy_x", m_enemy.getPos().x}, {"position_enemy_y", m_enemy.getPos().y}, {"position_enemy_z", m_enemy.getPos().z}, {"yaw", m_player.Yaw}}}};

        //add new score
        if (exists(jscores, m_player.getUsername())) {
            cout << "User already exist" << endl;

            jscores.update(jscores_update);
        }

        else
            jscores += {m_player.getUsername(), {{"score", m_player.getScore()}, {"life", m_player.getLife()}, {"position_x", m_player.getPos().x}, {"position_y", m_player.getPos().y}, {"position_z", m_player.getPos().z}, {"position_enemy_x", m_enemy.getPos().x}, {"position_enemy_y", m_enemy.getPos().y}, {"position_enemy_z", m_enemy.getPos().z}, {"yaw", m_player.Yaw}}};

        // write prettified JSON to another file
        std::ofstream writtingFile(BIN_PATH + "/assets/scores.json"); // TODO : gestion erreur
        if (writtingFile.is_open()) {
            writtingFile << jscores << std::endl;
            writtingFile.close();
        }
        else
            cerr << "Unable to open file WRITE";
    }
    else
        cerr << "Unable to open file READ";
};

bool Game::exists(const json& j, const std::string& key)
{
    return j.find(key) != j.end();
};