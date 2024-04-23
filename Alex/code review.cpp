#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream> 
using namespace std;


class User {
public:

  string username;
  string displayname;
  string state;
  vector<string> friends;

  User (string username, string displayname, string state, vector<string> friends) {
    this->username = username;
    this->displayname = displayname;
    this->state = state;
    this->friends = friends;
  }

};

class Post {
public:

  string postID;
  string userID;
  string visibility;

  Post (string postID, string userID, string visibility) {
    this->postID = postID;
    this->userID = userID;
    this->visibility = visibility;
  }

};

vector<User> loadUserInfo (const string & userFilePath_input) {
  vector<User> tempAllUsers;
  fstream userFile;
  userFile.open(userFilePath_input);
  if (!userFile.is_open()) {
    cerr << "Error opening file " << userFilePath_input << "\n";

  }

  string file_username, file_displayname, file_state, line;
  vector<string> file_friends;
  while (getline(userFile, line)) {

    int position_holder = 0;
    position_holder = line.find(";");
    file_username = line.substr(0, position_holder);
    line.erase(0, position_holder+1);

    position_holder = line.find(";");
    file_displayname = line.substr(0, position_holder);
    line.erase(0, position_holder+1);

    position_holder = line.find(";");
    file_state = line.substr(0, position_holder);
    line.erase(0, position_holder+1);

    int lineSize = line.size();
    line = line.substr(1, lineSize-2);
    file_friends.clear();

    while ((position_holder = line.find(",")) != string::npos) {
      file_friends.push_back(line.substr(0, position_holder));
      line.erase(0, position_holder+1);
    }
    file_friends.push_back(line);

    User newUser(file_username, file_displayname, file_state, file_friends);
    tempAllUsers.push_back(newUser);
    
  }
  return tempAllUsers;
  userFile.close();
  
}

vector<Post> loadPostInfo (const string & postFilePath_input) {
  vector<Post> tempAllPosts;
  fstream postFile;
  postFile.open(postFilePath_input);
  if (!postFile.is_open()) {
    cerr << "Error opening file " << postFilePath_input << "\n";
  }

  string file_postID, file_userID, file_visibility, line2;
  while (getline(postFile, line2)) {
    int position_holder2 = 0;
    position_holder2 = line2.find(";");
    file_postID = line2.substr(0, position_holder2);
    line2.erase(0, position_holder2+1);

    position_holder2 = line2.find(";");
    file_userID = line2.substr(0, position_holder2);
    line2.erase(0, position_holder2+1);

    position_holder2 = line2.find(";");
    file_visibility = line2.substr(0, position_holder2);
    line2.erase(0, position_holder2+1);

    Post newPost(file_postID, file_userID, file_visibility);
    tempAllPosts.push_back(newPost);
    
  }
  return tempAllPosts;
  postFile.close();
}

int main() {
  vector<User> AllUsers;
  vector<Post> AllPosts;
  int UserSelectedOption;
  string continue_check;

  do {
    do {
      cout << "Please select an option: " << endl;
      cout << "Enter 1: to load data " << "\n" << "Enter 2: to check visibility " << "\n" 
        << "Enter 3: to retrieve posts " << "\n" << "Enter 4: to search for users by location " 
        << "\n" << "Enter 5: to terminate program " << "\n";
      cin >> UserSelectedOption;
    } while (UserSelectedOption < 1 || UserSelectedOption > 5);
  
    
    if (UserSelectedOption == 1) {
      string userFilePath_input, postFilePath_input;
      cout << "Please enter a file path for users: " << "\n";
      cin >> userFilePath_input;
      cout << "Please enter a file path for posts: " << "\n";
      cin >> postFilePath_input;

      AllUsers = loadUserInfo(userFilePath_input);
      AllPosts = loadPostInfo(postFilePath_input);
      
    } else if (UserSelectedOption == 2) {
      string postID_input, username_input;
      bool Access = false;
      cout << "Please enter a post ID: \n";
      cin >> postID_input;
      cout << "Please enter a username : \n";
      cin >> username_input;

      for (Post post: AllPosts) {
        if (post.postID == postID_input) {
          if (post.visibility == "public") {
            Access = true;
            break;
          } else if (post.visibility == "friend") {
            for (User user: AllUsers) {
              if (user.username == username_input) {
                if (user.username == post.userID) {
                  Access = true;
                  break;
                } else {
                  for (const auto & friendName : user.friends) {
                    if (friendName == post.userID) {
                      Access = true;
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }

      if (Access == true) {
          cout << "Access Permitted \n";
      } else if (Access == false) {
          cout << "Access Denied \n";
      }
      
    } else if (UserSelectedOption == 3) {
      string userName_input;
      cout << "Please enter a user name: \n";
      cin >> userName_input;

      for (Post post: AllPosts) {
        if (post.userID == userName_input){
          cout << post.postID << "\n";
        }
      }
  
    } else if (UserSelectedOption == 4) {
      string location_input;
      cout << "Please enter a location: \n";
      cin >> location_input;

      for (User user: AllUsers) {
        if (user.state == location_input) {
          cout << user.displayname << "\n";
        }
      }
      
    } else if (UserSelectedOption == 5) {
      cout << "Program Terminated..." << endl;
      break;
    }

    cout << "Would you like to continue? (y/n) \n";
    cin >> continue_check;
  } while (continue_check != "n");
  
}
