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

void loadUserData(const string& filename) {
    ifstream file(filename);
    string entryLine;
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
}

void loadPostData(const string& filename){
    ifstream file(filename);
    string entryLine;
    while(getline(file, entryLine)){
        stringstream ss(entryLine);
        Post post;
        getline(ss, post.postId, ';');
        getline(ss, post.userId, ';');
        getline(ss, post.visibility);
        posts.push_back(post);
    }
}

string checkVisibility(const string& postId, const string& username){
    for(Post post : posts){
        if(post.postId == postId){
            if(post.visibility == "public"){
                return "Access Permitted";
            }
            for(User user : users){
                if(user.username == post.userId){
                    if(find(user.friends.begin(), user.friends.end(), username) != user.friends.end()){
                        return "Access Permitted";
                    }
                    break;
                }
            }
        }
    }
    return "Access Denied";
}
vector<string> retrievePosts(string username) {
    vector<string> visiblePosts;
    for (const Post& post : posts) {
        if (post.userId != username) {
            if (post.visibility == "public") {
                visiblePosts.push_back(post.postId);
            } else if (post.visibility == "friend") {
                for(User user : users){
                  if(user.username == post.userId){
                    for(string friendName : user.friends){
                      if(friendName == username){
                          visiblePosts.push_back(post.postId);
                          //break;
                      }
                      
                    }
                  }
                }
            }
        }
    }
    return visiblePosts;
}
vector<string> searchUsersByLocation(const string& state){
    vector<string> matchedUsers;
    for(User user : users){
        if(user.state == state){
            matchedUsers.push_back(user.displayName);
        }
    }
    return matchedUsers;
}

int main(){
    string userFilePath, postFilePath;
    int choice = 0;
    bool loop = true;
    string userFile, postFile, postId, username, state;
    cout << "Please Select a Numbered Option From the Following:\n";
    cout << "1. Load input data\n";
    cout << "2. Check visibility\n";
    cout << "3. Retrieve posts\n";
    cout << "4. Search users by location\n";
    cout << "5. Exit\n";
    while(loop){
        cin >> choice;
        cout << endl;
        if(choice == 1){
            cout << "Enter user file path: ";
            cin >> userFile;
            cout << "Enter post file path: ";
            cin >> postFile;
            loadUserData(userFile);
            loadPostData(postFile);
            cout << "Data from user and post successfully loaded!\n";
          cout << "Please enter new choice:";
        }else if(choice == 2){
            cout << "Enter a postID: ";
            cin >> postId;
            cout << "Enter the username: ";
            cin >> username;
            cout << checkVisibility(postId, username) << endl;
            cout << "Please enter new choice:";
        }else if(choice == 3){
            cout << "Enter a username: ";
            cin >> username;
            for(string postId : retrievePosts(username)){
                cout << postId << endl;
            }
            cout << "Please enter new choice:";
        }else if(choice == 4){
            cout << "Enter user state location (state initials): ";
            cin >> state; 
            for(string displayName : searchUsersByLocation(state)){
                cout << displayName << endl;
            }
            cout << "Please enter new choice:";
        }else if(choice == 5){
            cout << "Exiting Program." << endl;
            loop = false;
        }else{
          cout << "Invalid choice. Please try again." << endl;
          cout << "Please enter new choice:";
        }
    }
    return 0;

}