#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct User {
    string username;
    string displayName;
    string state;
    vector<string> friends;
};

struct Post {
    string postId;
    string userId;
    string visibility;
};

vector<User> users;
vector<Post> posts;
bool dataLoaded = false;  // Flag to check if data is loaded

void loadUserData(string filename) {
    ifstream file(filename);
    string entryLine;
    if (!file) {
        cout << "Failed to open user file." << endl;
        return;
    }
    while (getline(file, entryLine)) {
        stringstream ss(entryLine);
        User user;
        string friends, friendName;
        getline(ss, user.username, ';');
        getline(ss, user.displayName, ';');
        getline(ss, user.state, ';');
        getline(ss, friends);
        friends = friends.substr(1, friends.length() - 2);

        stringstream fs(friends);
        while (getline(fs, friendName, ',')) {
            if (!friendName.empty()) {
                user.friends.push_back(friendName);
            }
        }
        users.push_back(user);
    }
    dataLoaded = true;  // Set data loaded flag to true after successful load
}

void loadPostData(string filename){
    ifstream file(filename);
    string entryLine;
    if (!file) {
        cout << "Failed to open post file." << endl;
        return;
    }
    while(getline(file, entryLine)){
        stringstream ss(entryLine);
        Post post;
        getline(ss, post.postId, ';');
        getline(ss, post.userId, ';');
        getline(ss, post.visibility);
        posts.push_back(post);
    }
}

string checkVisibility(string postId, string username){
    if (!dataLoaded) {
        return "Please load data first";
    }
    for(const Post& post : posts){
        if(post.postId == postId){
            if(post.visibility == "public"){
                return "Access Permitted";
            } else {
                // Check if the username is in the friends list of the post's user
                auto it = find_if(users.begin(), users.end(), [&post](const User& u) {
                    return u.username == post.userId;
                });
                if (it != users.end() && find(it->friends.begin(), it->friends.end(), username) != it->friends.end()) {
                    return "Access Permitted";
                }
            }
        }
    }
    return "Access Denied";
}

vector<string> retrievePosts(string username) {
    if (!dataLoaded) {
        return {"Please load data first"};
    }
    vector<string> visiblePosts;
    for (const Post& post : posts) {
        if (post.userId != username || 
            (post.visibility == "friend" && find_if(users.begin(), users.end(), [&post, &username](const User& u) {
                return u.username == post.userId && find(u.friends.begin(), u.friends.end(), username) != u.friends.end();
            }) != users.end())) {
            visiblePosts.push_back(post.postId);
        }
    }
    return visiblePosts;
}

vector<string> searchUsersByLocation(string state){
    if (!dataLoaded) {
        return {"Please load data first"};
    }
    vector<string> matchedUsers;
    for(const User& user : users){
        if(user.state == state){
            matchedUsers.push_back(user.displayName);
        }
    }
    return matchedUsers;
}

int main(){
    string userFile, postFile;
    int choice = 0;
    bool loop = true;
    cout << "Please Select a Numbered Option From the Following:\n";
    cout << "1. Load input data\n";
    cout << "2. Check visibility\n";
    cout << "3. Retrieve posts\n";
    cout << "4. Search users by location\n";
    cout << "5. Exit\n";
    while(loop){
        cin >> choice;
        cout << endl;
        switch (choice) {
            case 1:
                cout << "Enter user file path: ";
                cin >> userFile;
                cout << "Enter post file path: ";
                cin >> postFile;
                loadUserData(userFile);
                loadPostData(postFile);
                cout << "Data from user and post files successfully loaded!\n";
                break;
            case 2:
                if (!dataLoaded) {
                    cout << "Please load data first" << endl;
                } else {
                    string postId, username;
                    cout << "Enter a postID: ";
                    cin >> postId;
                    cout << "Enter the username: ";
                    cin >> username;
                    cout << checkVisibility(postId, username) << endl;
                }
                break;
            case 3:
                if (!dataLoaded) {
                    cout << "Please load data first" << endl;
                } else {
                    string username;
                    cout << "Enter a username: ";
                    cin >> username;
                    vector<string> posts = retrievePosts(username);
                    for (const string& postId : posts) {
                        cout << postId << endl;
                    }
                }
                break;
            case 4:
                if (!dataLoaded) {
                    cout << "Please load data first" << endl;
                } else {
                    string state;
                    cout << "Enter user state location (state initials): ";
                    cin >> state;
                    vector<string> names = searchUsersByLocation(state);
                    for (const string& name : names) {
                        cout << name << endl;
                    }
                }
                break;
            case 5:
                cout << "Exiting Program." << endl;
                loop = false;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
    return 0;
}
