import numpy as np
import random

# Define the environment (grid world)
class GridWorld:
    def __init__(self, grid, start, goal, obstacles):
        self.grid = grid
        self.start = start
        self.goal = goal
        self.obstacles = obstacles
        self.state = start
        self.actions = [(0, 1), (1, 0), (0, -1), (-1, 0)] # action_symbols = ['→', '↓', '←', '↑']
        self.action_names = ['R', 'D', 'L', 'U']
        self.rows = len(grid)
        self.cols = len(grid[0]) if self.rows > 0 else 0

    def reset(self):
        """Reset the state to the starting point."""
        self.state = self.start
        return self.state

    def is_valid_position(self, x, y):
        """Check if position is within bounds and not an obstacle."""
        if x < 0 or x >= self.rows or y < 0 or y >= self.cols:
            return False
        if self.grid[x][y] == 0:
            return False
        return True

    def step(self, action):
        """
        Execute an action and return (next_state, reward, done).
        Points: 100 for goal, -10 for invalid move, small negative value for each step
        
        Args:
        - action: Index of the action (0=Right, 1=Down, 2=Left, 3=Up)
        
        Returns:
        - next_state: The new state after taking the action
        - reward: The reward received
        - done: Whether the episode is finished (reached goal)
        """
        dx, dy = self.actions[action]
        x, y = self.state
        nx, ny = x + dx, y + dy
        
        # Check if the move is valid
        if self.is_valid_position(nx, ny):
            self.state = (nx, ny)
            
            # Check if reached goal
            if self.state == self.goal:
                # Large positive reward for reaching the goal
                return self.state, 100, True
            else:
                # Negative reward based on cell cost (to minimize path cost)
                reward = -self.grid[nx][ny]
                return self.state, reward, False
        else:
            # Invalid move: stay in place, receive penalty
            return self.state, -10, False


# Q-Learning Algorithm
class QLearning:
    def __init__(self, env, alpha=0.1, gamma=0.9, epsilon=0.1):
        self.env = env
        self.alpha = alpha  # Learning rate
        self.gamma = gamma  # Discount factor
        self.epsilon = epsilon  # Exploration rate
        
        # Initialize Q-table with zeros
        # Shape: (rows, cols, num_actions)
        self.q_table = np.zeros((env.rows, env.cols, len(env.actions)))

    def choose_action(self, state):
        """
        Use random action for exploration and the best action based on Q-values for exploitation.
        
        Args:
        - state: Current state (row, col)
        
        Returns:
        - action: Index of the chosen action
        """
        if random.random() < self.epsilon:
            # Exploration: choose random action
            return random.randint(0, len(self.env.actions) - 1)
        else:
            # Exploitation: choose best action based on Q-values
            x, y = state
            q_values = self.q_table[x, y, :]
            # Break ties randomly
            max_q = np.max(q_values)
            best_actions = np.where(q_values == max_q)[0]
            return random.choice(best_actions)

    def update_q_table(self, state, action, reward, next_state):
        """
        Implement the Q-value update using the Q-learning formula.
        Q(s,a) <- Q(s,a) + alpha * [R + gamma * max_a' Q(s',a') - Q(s,a)]
        
        Args:
        - state: Current state (row, col)
        - action: Action taken
        - reward: Reward received
        - next_state: Next state after taking action
        """
        x, y = state
        nx, ny = next_state
        
        # Current Q-value
        current_q = self.q_table[x, y, action]
        
        # Maximum Q-value for next state
        max_next_q = np.max(self.q_table[nx, ny, :])
        
        # Q-learning update
        td_target = reward + self.gamma * max_next_q
        td_error = td_target - current_q
        self.q_table[x, y, action] = current_q + self.alpha * td_error

    def train(self, episodes=1000, max_steps_per_episode=1000):
        """
        Train the agent using Q-Learning over a specified number of episodes.
        
        Args:
        - episodes: Number of training episodes
        - max_steps_per_episode: Maximum steps allowed per episode
        
        Returns:
        - rewards_per_episode: List of total rewards for each episode
        """
        rewards_per_episode = []
        
        for episode in range(episodes):
            state = self.env.reset()
            total_reward = 0
            done = False
            steps = 0
            
            while not done and steps < max_steps_per_episode:
                # Choose action
                action = self.choose_action(state)
                
                # Take action
                next_state, reward, done = self.env.step(action)
                
                # Update Q-table
                self.update_q_table(state, action, reward, next_state)
                
                # Move to next state
                state = next_state
                total_reward += reward
                steps += 1
            
            rewards_per_episode.append(total_reward)
            
            # Observing proceess
            if (episode + 1) % 100 == 0:
                avg_reward = np.mean(rewards_per_episode[-100:])
                #print(f"Episode {episode + 1}/{episodes}, Avg Reward (last 100): {avg_reward:.2f}")
        
        return rewards_per_episode

    def get_policy(self):
        """
        Extract the learned policy from the Q-table.
        
        Returns:
        - policy: 2D array with best action index for each state
        """
        policy = np.zeros((self.env.rows, self.env.cols), dtype=int)
        
        for x in range(self.env.rows):
            for y in range(self.env.cols):
                # Get best action for this state
                policy[x, y] = np.argmax(self.q_table[x, y, :])
        
        return policy

    def get_path(self, start=None, max_steps=100):
        """
        Get the path from start to goal using the learned policy.
        
        Args:
        - start: Starting state (defaults to environment start)
        - max_steps: Maximum steps to prevent infinite loops
        
        Returns:
        - path: List of (state, action_name) tuples
        - reached_goal: Whether the goal was reached
        """
        if start is None:
            start = self.env.start
        
        path = []
        state = start
        visited = set()
        
        for _ in range(max_steps):
            if state == self.env.goal:
                return path, True
            
            if state in visited:
                print("Warning: Loop detected")
                return path, False
            visited.add(state)
            
            x, y = state
            action = np.argmax(self.q_table[x, y, :])
            action_name = self.env.action_names[action]
            path.append((state, action_name))
            
            # Move to next state
            dx, dy = self.env.actions[action]
            next_x, next_y = x + dx, y + dy
            
            if self.env.is_valid_position(next_x, next_y):
                state = (next_x, next_y)
            else:
                # Invalid move
                return path, False
        
        return path, state == self.env.goal


def read_qlearning_file(file_path):
    """
    Read Q-learning training/test file format.
    
    Args:
    - file_path: Path to the file
    
    Returns:
    - instances: List of dictionaries with grid information
    """
    instances = []
    
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    i = 0
    while i < len(lines):
        # Skip blank lines
        while i < len(lines) and lines[i].strip() == "":
            i += 1
        
        if i >= len(lines): break
        
        # Read start point
        start_line = lines[i].strip()
        start = tuple(map(int, start_line.split()))
        i += 1
        
        # Read end point
        end_line = lines[i].strip()
        end = tuple(map(int, end_line.split()))
        i += 1
        
        # Read grid size
        size_line = lines[i].strip()
        size = int(size_line)
        i += 1
        
        # Read grid
        grid = []
        obstacles = []
        for row in range(size):
            grid_line = lines[i].strip()
            row_values = list(map(int, grid_line.split()))
            grid.append(row_values)
            for col, val in enumerate(row_values):
                if val == 0: obstacles.append((row, col))
            i += 1
        
        instances.append({
            'start': start,
            'end': end,
            'size': size,
            'grid': grid,
            'obstacles': obstacles
        })
    
    return instances