#Joshua Morrison @03061335

# Define the Boggle class to manage the game logic

class Boggle: 
    def __init__(self, grid, dictionary):
        self.grid = grid  # Initialize the grid for the game
        self.dictionary = set(dictionary)  # Convert dictionary to a set for faster lookups
        self.solution = []  # List to store valid words found in the grid

    # Setters and getters for the grid, dictionary, and solution

    def setGrid(self, grid):
        self.grid = grid  # Set the grid for the game
    
    def setDictionary(self, dictionary):
        self.dictionary = set(dictionary)  # Set the dictionary (converted to a set)

    def getSolution(self):
        return self.solution  # Return the list of words found

# Define a helper function to get valid neighboring cells for a given position

    def get_neighbors(self, row, col):
        # Directions representing all possible neighbor moves: (row_delta, col_delta)
        directions = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
        neighbors = []  # List to store valid neighboring cells
        
        # Iterate through each direction to find neighboring cells
        for dr, dc in directions:
            r = row + dr  # Calculate new row position
            c = col + dc  # Calculate new column position
            # Check if the new position is within grid boundaries
            if 0 <= r < len(self.grid) and 0 <= c < len(self.grid[0]):
                neighbors.append((r, c))  # Append valid neighbor coordinates to the list
        return neighbors

# Define the DFS function to explore all word possibilities from each cell

    def dfs(self, row, col, visited, prefix):
        visited.add((row, col))  # Mark the current cell as visited
        letter = self.grid[row][col]  # Get the letter at the current cell
        
        # Build the new word by adding the letter to the current prefix
        new_word = prefix + letter

        # If the new word is at least 3 letters long and is in the dictionary
        if len(new_word) >= 3 and new_word in self.dictionary:
            if new_word not in self.solution:
                self.solution.append(new_word)  # Add the word to the solution if it's not already there
        
        # Explore all neighboring cells recursively
        for r, c in self.get_neighbors(row, col):
            if (r, c) not in visited:  # If the neighbor hasn't been visited yet
                self.dfs(r, c, visited.copy(), new_word)  # Recursively call DFS with the new position

        visited.remove((row, col))  # Backtrack to explore other possible paths

# Solve the Boggle puzzle by starting DFS from each cell in the grid

    def solve(self):
        # Iterate through every cell in the grid
        for row in range(len(self.grid)):
            for col in range(len(self.grid[0])):
                visited = set()  # Keep track of visited cells for the current word path
                self.dfs(row, col, visited, "")  # Start DFS from the current cell

# Main function to run the Boggle game

def main():
    # Example grid
    grid = [["G", "I", "Z"], 
            ["U", "E", "K"], 
            ["Q", "S", "E"]]
    
    # Example dictionary
    dictionary = ["GEEKS", "FOR", "QUIZ", "GO"]

    # Create an instance of Boggle with the grid and dictionary
    mygame = Boggle(grid, dictionary)
    
    # Solve the Boggle puzzle
    mygame.solve()
    
    # Print the found solution
    print(mygame.getSolution())

if __name__ == "__main__":
    main()