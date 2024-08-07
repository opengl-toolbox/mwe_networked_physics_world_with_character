#ifndef MWE_NETWORKING_CLIENT_HPP
#define MWE_NETWORKING_CLIENT_HPP

#include "enet.h"
#include "interaction/mouse/mouse.hpp"
#include "interaction/multiplayer_physics/physics.hpp"
#include "networked_input_snapshot/networked_input_snapshot.hpp"
#include "interaction/camera/camera.hpp"
#include "expiring_data_container/expiring_data_container.hpp"
#include "networked_character_data/networked_character_data.hpp"
#include <string>

class ClientNetwork {
  public:
    ClientNetwork(NetworkedInputSnapshot *input_snapshot, std::string &ip_address, int port);
    ~ClientNetwork();

    uint64_t id = -1;
    NetworkedInputSnapshot *input_snapshot;
    ENetHost *client;
    ENetPeer *server_connection;
    std::string server_ip_address;
    int server_port;
    NetworkedCharacterData most_recent_client_game_state_update;

    std::function<void(double)>
    network_step_closure(int send_frequency_hz, Physics &physics, Camera &camera, Mouse &mouse,
                         std::unordered_map<uint64_t, NetworkedCharacterData> &client_id_to_character_data,
                         ExpiringDataContainer<NetworkedInputSnapshot> &processed_input_snapshot_history);

    void handle_network_event(ENetEvent event, Physics &physics, Camera &camera, Mouse &mouse,
                              std::unordered_map<uint64_t, NetworkedCharacterData> &client_id_to_character_data,
                              ExpiringDataContainer<NetworkedInputSnapshot> &processed_input_snapshot_history);

    void process_game_state_update(NetworkedCharacterData *game_update, int game_update_length, Physics &physics,
                                   Camera &camera, Mouse &mouse,
                                   std::unordered_map<uint64_t, NetworkedCharacterData> &client_id_to_character_data,
                                   ExpiringDataContainer<NetworkedInputSnapshot> &processed_input_snapshot_history);

    int start_network_loop(int send_frequency_hz, Physics &physics, Camera &camera, Mouse &mouse,
                           std::unordered_map<uint64_t, NetworkedCharacterData> &client_id_to_character_data,
                           ExpiringDataContainer<NetworkedInputSnapshot> &processed_input_snapshot_history);

    void send_input_snapshot(ExpiringDataContainer<NetworkedInputSnapshot> &processed_input_snapshot_history);

    std::mutex reconcile_mutex;
    void reconcile_local_game_state_with_server_update(
        NetworkedCharacterData &networked_character_data, Physics &physics, Camera &camera, Mouse &mouse,
        std::unordered_map<uint64_t, NetworkedCharacterData> &client_id_to_character_data,
        ExpiringDataContainer<NetworkedInputSnapshot> &processed_input_snapshot_history,
        JPH::Vec3 &authorative_position, JPH::Vec3 &authorative_velocity);

    void update_local_client_with_game_state(
        NetworkedCharacterData &networked_character_data, Physics &physics, Camera &camera, Mouse &mouse,
        std::unordered_map<uint64_t, NetworkedCharacterData> &client_id_to_character_data,
        ExpiringDataContainer<NetworkedInputSnapshot> &processed_input_snapshot_history);
    void initialize_client_network();
    void attempt_to_connect_to_server();
    void disconnect_from_server();

  private:
    unsigned int input_snapshot_to_binary();
};

#endif // MWE_NETWORKING_CLIENT_HPP
