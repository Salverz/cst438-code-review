import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class Main {
    Main() {
        this.users = new ArrayList<>();
        this.posts = new ArrayList<>();
    }

    class User {
        String username;
        String displayName;
        String state;
        ArrayList<String> friends;

        User() {
            this.username = "";
            this.displayName = "";
            this.state = "";
            this.friends = new ArrayList<>();
        }
    }

    class Post {
        String postId;
        String userId;
        String visibility;

        Post() {
            this.postId = "";
            this.userId = "";
            this.visibility = "";
        }
    }

    ArrayList<User> users;
    ArrayList<Post> posts;

    public static void main(String[] args) {
        Main main = new Main();
        int choice;
        do {
            // Print options
            System.out.println("1. Load input data.");
            System.out.println("2. Check visibility.");
            System.out.println("3. Retrieve Posts.");
            System.out.println("4. Search users by location:");
            System.out.println("5. Exit:");
            System.out.print("Select an action: ");
            Scanner s = new Scanner(System.in);
            choice = s.nextInt();

            // Run function to carry out selected action
            switch (choice) {
                case 1:
                    main.loadInputData(main);
                    break;
                case 2:
                    main.checkVisibility(main);
                    break;
                case 3:
                    main.retrievePosts(main);
                    break;
                case 4:
                    main.searchUsersByLocation(main);
                    break;
                case 5:
                    break;
            }
            System.out.println();
        } while (choice != 5);
    }

    // Load in data from the post-info.txt and user-info.txt files
    private void loadInputData(Main main) {
        // Load the user-info file
        String filePath = System.getProperty("user.dir") + "/src/user-info.txt";
        File file = new File(filePath);
        try {
            Scanner s = new Scanner(file);
            // Loop through each line of the file
            while (s.hasNextLine()) {
                User user = new User();
                String line = s.nextLine();
                // Split the line on ; and assign each of the arguments to a user object
                String[] arguments = line.split(";");
                user.username = arguments[0];
                user.displayName = arguments[1];
                user.state = arguments[2];
                String[] friends = arguments[3]
                        .replace("[", "")
                        .replace("]", "")
                        .split(",");
                user.friends.addAll(Arrays.asList(friends));
                // Add the user to the user list
                main.users.add(user);
            }
            s.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        // Load the post-info file
        filePath = System.getProperty("user.dir") + "/src/post-info.txt";
        file = new File(filePath);
        try {
            Scanner s = new Scanner(file);
            // Loop through each line of the file
            while (s.hasNextLine()) {
                Post post = new Post();
                String line = s.nextLine();
                // Split the line on ; and assign each of the arguments to a user object
                String[] arguments = line.split(";");
                post.postId = arguments[0];
                post.userId = arguments[1];
                post.visibility = arguments[2];
                // Add the user to the post list
                main.posts.add(post);
            }
            s.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    // Check whether a specific user can see a specific post
    private void checkVisibility(Main main) {
        Scanner s = new Scanner(System.in);
        System.out.print("Enter a post ID: ");
        String postId = s.next();
        System.out.print("Enter a username: ");
        String username = s.next();

        // Loop through all posts to find the requested post
        for (Post post : main.posts) {
            if (!post.postId.equals(postId)) {
                continue;
            }

            if (post.visibility.equals("public")) {
                System.out.println("Access Permitted");
                return;
            }

            // If the requested user is the poster
            if (post.userId.equals(username)) {
                System.out.println("Access Permitted");
                return;
            }

            // If the post is "friends only", loop over all users to find the requested user
            for (User user : main.users) {
                if (!post.userId.equals(user.username)) {
                    continue;
                }

                // Check if the user is a friend of the poster
                for (String friend : user.friends) {
                    if (!friend.equals(username)) {
                        continue;
                    }

                    System.out.println("Access Permitted");
                    return;
                }
            }
        }
        // If the user or post could not be found
        // or the post is private and the requested user is not a friend to the poster
        System.out.println("Access Denied");
    }

    // Get all the posts that an input user can see
    private void retrievePosts(Main main) {
        Scanner s = new Scanner(System.in);
        System.out.print("Enter a username: ");
        String username = s.next();

        // Get the User object associated with the input username
        User inputUser = null;
        for (User user : main.users) {
            if (user.username.equals(username)) {
                inputUser = user;
                break;
            }
        }

        if (inputUser == null) {
            System.out.println("User not found");
            return;
        }
        ArrayList<String> visibilePostIds = new ArrayList<>();
        for (Post post : main.posts) {
            // If post is public
            if (post.visibility.equals("public")) {
                visibilePostIds.add(post.postId);
                continue;
            }

            // If the user is the poster
            if (post.userId.equals(username)) {
                visibilePostIds.add(post.postId);
                continue;
            }

            // Check if the user is a friend of the poster
            for (String friend : inputUser.friends) {
                if (friend.equals(post.userId)) {
                    visibilePostIds.add(post.postId);
                }
            }
        }

        if (visibilePostIds.isEmpty()) {
            System.out.println("No posts are visible");
            return;
        }

        // Print out results
        System.out.print(visibilePostIds.get(0));
        for (int i = 1; i < visibilePostIds.size(); i++) {
            System.out.printf(", %s", visibilePostIds.get(i));
        }
        System.out.println();
    }

    // Find all users in an input state
    private void searchUsersByLocation(Main main) {
        System.out.print("Enter a state: ");
        Scanner s = new Scanner(System.in);
        String state = s.next().toLowerCase();  // Convert to lowercase to make search non-case-sensitive

        // Loop over all users to check if their state matches the input state
        ArrayList<String> userDisplayNames = new ArrayList<>();
        for (User user : main.users) {
            if (user.state.toLowerCase().equals(state)) {
                userDisplayNames.add(user.displayName);
            }
        }

        if (userDisplayNames.isEmpty()) {
            System.out.println("No users are in that state");
            return;
        }

        // Print out results
        System.out.print(userDisplayNames.get(0));
        for (int i = 1; i < userDisplayNames.size(); i++) {
            System.out.printf(", %s", userDisplayNames.get(i));
        }
        System.out.println();
    }
}
