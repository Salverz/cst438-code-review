import java.io.*;
import java.util.*;

class User {
    String username;
    String displayName;
    String state;
    Set<String> friends;

    // User Constructor
    public User(String username, String displayName, String state, Set<String> friends) {
        this.username = username;
        this.displayName = displayName;
        this.state = state;
        this.friends = friends;
    }
}

class Post {
    String postId;
    String userId;
    String visibility;

    //Post Constructor
    public Post(String postId, String userId, String visibility) {
        this.postId = postId;
        this.userId = userId;
        this.visibility = visibility;
    }
}

public class Main
{
    private static List<User> users = new ArrayList<>();
    private static List<Post> posts = new ArrayList<>();

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        boolean exit = false;

        // User Input While Loop & SwitchCase for Choosing Desired Option
        while (!exit) {
            System.out.println("1. Load input data");
            System.out.println("2. Check visibility");
            System.out.println("3. Retrieve posts");
            System.out.println("4. Search users by location");
            System.out.println("5. Exit");
            System.out.print("Enter your choice: ");

            int choice = sc.nextInt();
            sc.nextLine();

            switch (choice) {
                case 1:
                    loadData();
                    break;
                case 2:
                    checkVisibility(sc);
                    break;
                case 3:
                    retrievePosts(sc);
                    break;
                case 4:
                    searchUsersByLocation(sc);
                    break;
                case 5:
                    exit = true;
                    break;
                default:
                    System.out.println("Invalid choice. Please try again.");
            }
        }
    }

    //Load Data from File
    private static void loadData() {
        readUser("src/user-info.txt");
        readPost("src/post-info.txt");
        System.out.println("Input data loaded successfully.");
    }

    //Read User Info Data from File
    private static void readUser(String filename) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(";");
                String username = parts[0];
                String displayName = parts[1];
                String state = parts[2];
                Set<String> friends = new HashSet<>(Arrays.asList(parts[3].replaceAll("[\\[\\]]", "").split(", ")));
                users.add(new User(username, displayName, state, friends));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //Read Post Info from File
    private static void readPost(String filename) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(";");
                String postId = parts[0];
                String userId = parts[1];
                String visibility = parts[2];
                posts.add(new Post(postId, userId, visibility));
            }
        } catch (IOException e) // File Reader Exceptions Handler .
        {
            e.printStackTrace();
        }
    }

    // Method to Check Visibility of a Post relative to the User
    private static boolean canUserViewPost(String username, String postId) {
        for (Post post : posts) {
            if (post.postId.equals(postId))
            {
                if (post.visibility.equals("public"))
                {
                    return true;
                }
                else if (post.visibility.equals("friend"))
                {
                    User user = getUserByUsername(username);
                    return user != null && user.friends.contains(post.userId);
                }
            }
        }
        return false;
    }

    // Method to retrieve accessible posts from User .
    private static List<Post> getAccessiblePosts(String username) {
        List<Post> accessiblePosts = new ArrayList<>();
        for (Post post : posts) {
            if (post.userId.equals(username) || canUserViewPost(username, post.postId)) {
                accessiblePosts.add(post);
            }
        }
        return accessiblePosts;
    }


    // Method to get users by location
    private static List<User> getUsersByLocation(String state) {
        List<User> usersInState = new ArrayList<>();
        for (User user : users) {
            if (user.state.equals(state)) {
                usersInState.add(user);
            }
        }
        return usersInState;
    }

    // Method to get users by Username
    private static User getUserByUsername(String username) {
        for (User user : users) {
            if (user.username.equals(username)) {
                return user;
            }
        }
        return null;
    }

    // Check the Visibility of a post .
    private static void checkVisibility(Scanner sc) {
        System.out.print("Enter post ID: ");
        String postId = sc.nextLine();
        System.out.print("Enter username: ");
        String username = sc.nextLine();

        boolean canViewPost = canUserViewPost(username, postId);
        System.out.println(canViewPost ? "Access Permitted" : "Access Denied");
    }

    // Retrieve Posts
    private static void retrievePosts(Scanner sc) {
        System.out.print("Enter username: ");
        String username = sc.nextLine();

        List<Post> accessiblePosts = getAccessiblePosts(username);
        System.out.println("Posts accessible to " + username + ":");
        for (Post post : accessiblePosts) {
            System.out.println(post.postId);
        }
    }

    //Method to search users by location
    private static void searchUsersByLocation(Scanner sc) {
        System.out.print("Enter state location: ");
        String state = sc.nextLine();

        List<User> usersInState = getUsersByLocation(state);
        System.out.println("Users in " + state + ":");
        for (User user : usersInState) {
            System.out.println(user.displayName);
        }
    }
}