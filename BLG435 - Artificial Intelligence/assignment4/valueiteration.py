import numpy as np

# 8-directional movement: (row_delta, col_delta)
ACTIONS = [
    (0, 1),
    (1, 1),
    (1, 0),
    (1, -1),
    (0, -1),
    (-1, -1),
    (-1, 0),
    (-1, 1),
]

ACTION_NAMES = ['R', 'RD', 'D', 'LD', 'L', 'LU', 'U', 'RU']

# Helper function to check if a state is within bounds and not a cliff
def is_valid_state(x, y, grid):
    """
    Checks if a state (x, y) is within the bounds of the grid
    and is not a cliff (grid value 0).
    
    Args:
    - x, y: Coordinates of the state.
    - grid: 2D grid representing the map.
    
    Returns:
    - True if the state is valid, False otherwise.
    """
    rows = len(grid)
    cols = len(grid[0]) if rows > 0 else 0
    
    # Check bounds
    if x < 0 or x >= rows or y < 0 or y >= cols:
        return False
    
    # Check if it's a cliff (value 0)
    if grid[x][y] == 0:
        return False
    
    return True


def can_move_diagonal(x, y, dx, dy, grid):
    """
    CONTROL: Diagonal movement is prohibited if one of the two directions that form it contains a cliff.

    Args:
    - x, y: Current state (row, col)
    - dx, dy: Movement direction
    - grid: 2D grid
    
    Returns:
    - True if diagonal move is allowed, False otherwise
    """
    # If not a diagonal move, always allowed (subject to destination check)
    if dx == 0 or dy == 0:
        return True
    
    # For diagonal move, check both adjacent cardinal directions
    # For example, RD (1, 1) requires R (0, 1) and D (1, 0) to not be cliffs
    adjacent1 = (x + dx, y)  # Vertical adjacent
    adjacent2 = (x, y + dy)  # Horizontal adjacent
    
    if not is_valid_state(adjacent1[0], adjacent1[1], grid):
        return False
    if not is_valid_state(adjacent2[0], adjacent2[1], grid):
        return False
    
    return True


# Main function for Value Iteration
def value_iteration(grid, start, end):
    """
    Performs value iteration to compute the optimal value function for all states.
    
    Args:
    - grid: 2D grid representing the map.
    - start: Starting state (x, y).
    - end: Goal state (x, y).
    
    Returns:
    - V: The value function for all states in the grid.
    """

    gamma = 0.9
    threshold = 1e-6
    max_iterations = 1000

    rows = len(grid)
    cols = len(grid[0]) if rows > 0 else 0
    
    # Initialization
    V = np.full((rows, cols), -np.inf)
    V[end[0], end[1]] = 0
    
    for iteration in range(max_iterations):
        V_old = V.copy()
        max_delta = 0
        
        for x in range(rows):
            for y in range(cols):
                # Skip cliffs and goal state
                if grid[x][y] == 0: continue # Cliff
                if (x, y) == end: continue # Goal state stays at 0  
                
                best_value = -np.inf
                
                for action_idx, (dx, dy) in enumerate(ACTIONS):
                    nx, ny = x + dx, y + dy
                    
                    # Valid state
                    if not is_valid_state(nx, ny, grid): continue 
                    if not can_move_diagonal(x, y, dx, dy, grid): continue    
                    
                    # Reward is negative cost of the destination cell
                    reward = -grid[nx][ny]
                    
                    # Bellman function
                    value = reward + gamma * V_old[nx, ny]
                    
                    if value > best_value:
                        best_value = value
                
                # Update the value
                if best_value > -np.inf:
                    V[x, y] = best_value
                    delta = abs(V[x, y] - V_old[x, y])
                    max_delta = max(max_delta, delta)
        
        # Check for convergence
        if max_delta < threshold:
            #print(f"Value Iteration converged after {iteration + 1} iterations")
            break
    else:
        #print(f"Value Iteration reached max iterations ({max_iterations})")
        pass
    
    return V


def get_policy(grid, V, end, gamma=0.9):
    """
    Extracts the optimal policy from the value function. 
    Kinda one time of iteration
    
    Args:
    - grid: 2D grid representing the map
    - V: Value function
    - end: Goal state
    - gamma: Discount factor
    
    Returns:
    - policy: 2D array with best action index for each state (-1 for invalid states)
    """
    rows = len(grid)
    cols = len(grid[0])
    policy = np.full((rows, cols), -1, dtype=int)
    
    for x in range(rows):
        for y in range(cols):
            if grid[x][y] == 0 or (x, y) == end:
                continue
            
            best_action = -1
            best_value = -np.inf
            
            for action_idx, (dx, dy) in enumerate(ACTIONS):
                nx, ny = x + dx, y + dy
                
                if not is_valid_state(nx, ny, grid):
                    continue
                
                if not can_move_diagonal(x, y, dx, dy, grid):
                    continue
                
                reward = -grid[nx][ny]
                value = reward + gamma * V[nx, ny]
                
                if value > best_value:
                    best_value = value
                    best_action = action_idx
            
            policy[x, y] = best_action
    
    return policy


def reconstruct_path(grid, start, end, V):
    """
    Reconstructs the optimal path from start to end using the value function.
    
    Args:
    - grid: 2D grid
    - start: Starting state (row, col)
    - end: Goal state (row, col)
    - V: Value function
    
    Returns:
    - path: List of action names (e.g., ['R', 'RD', 'D'])
    - total_cost: Total path cost
    """

    gamma = 0.9

    # V numeric, Policy actionic
    policy = get_policy(grid, V, end, gamma)
    
    path = []
    current = start
    total_cost = 0
    visited = set()
    max_steps = len(grid) * len(grid[0])
    
    while current != end and len(path) < max_steps:
        if current in visited:
            print("Warning: Loop detected in path reconstruction")
            break
        visited.add(current)
        
        x, y = current
        action_idx = policy[x, y]
        
        if action_idx == -1:
            print(f"No valid action at state {current}")
            return None, None
        
        path.append(ACTION_NAMES[action_idx])
        dx, dy = ACTIONS[action_idx]
        next_state = (x + dx, y + dy)
        total_cost += grid[next_state[0]][next_state[1]]
        current = next_state
    
    if current != end:
        print("Warning: Path did not reach the goal")
        return None, None
    
    return path, total_cost


def format_solution(path, total_cost):
    """
    Formats the solution in the required output format.
    
    Args:
    - path: List of action names
    - total_cost: Total path cost
    
    Returns:
    - Formatted string
    """
    if path is None:
        return "no path"
    
    path_str = "-".join(path)
    return f"{path_str} {len(path)} {total_cost}"