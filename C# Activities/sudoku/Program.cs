using System;
using System.Collections.Generic;
using System.IO;

namespace SudokuGame
{
    class Program
    {
        static int[,] board = new int[9, 9];
        static int[,] solution = new int[9, 9];
        static int[,] initialBoard = new int[9, 9];
        static int mistakes = 0;
        static string currentDifficulty = "";
        static bool gameWon = false;
        static bool gameLost = false;

        static void Main(string[] args)
        {
            while (true)
            {
                ShowMainMenu();
            }
        }

        static void ShowMainMenu()
        {
            Console.Clear();
            Console.WriteLine("===============================");
            Console.WriteLine("       SUDOKU GAME");
            Console.WriteLine("===============================");
            Console.WriteLine();
            Console.WriteLine("Selecciona la dificultad:");
            Console.WriteLine();
            Console.WriteLine("1. Facil (30 celdas vacias)");
            Console.WriteLine("2. Intermedio (40 celdas vacias)");
            Console.WriteLine("3. Dificil (50 celdas vacias)");
            Console.WriteLine("4. Muy Dificil (55 celdas vacias)");
            Console.WriteLine("5. Cargar Juego Guardado");
            Console.WriteLine("6. Salir");
            Console.WriteLine();
            Console.Write("Opcion: ");

            string? option = Console.ReadLine();
            if (option == null) option = "";

            if (option == "6")
            {
                Environment.Exit(0);
            }
            else if (option == "5")
            {
                LoadGame();
            }
            else if (option == "1" || option == "2" || option == "3" || option == "4")
            {
                StartNewGame(option);
            }
            else
            {
                Console.WriteLine("Opcion invalida. Presiona cualquier tecla...");
                Console.ReadKey();
            }
        }

        static void StartNewGame(string difficulty)
        {
            currentDifficulty = difficulty;
            mistakes = 0;
            gameWon = false;
            gameLost = false;

            int cellsToRemove = 30;
            if (difficulty == "2") cellsToRemove = 40;
            else if (difficulty == "3") cellsToRemove = 50;
            else if (difficulty == "4") cellsToRemove = 55;

            solution = GenerateSolvedBoard();
            board = CreatePuzzle(solution, cellsToRemove);
            
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    initialBoard[i, j] = board[i, j];
                }
            }

            PlayGame();
        }

        static void PlayGame()
        {
            while (!gameWon && !gameLost)
            {
                Console.Clear();
                Console.WriteLine("===============================");
                Console.WriteLine("       SUDOKU GAME");
                Console.WriteLine("===============================");
                Console.WriteLine();
                
                string diffName = "Facil";
                if (currentDifficulty == "2") diffName = "Intermedio";
                else if (currentDifficulty == "3") diffName = "Dificil";
                else if (currentDifficulty == "4") diffName = "Muy Dificil";
                
                Console.WriteLine("Dificultad: " + diffName);
                Console.WriteLine("Errores: " + mistakes + "/3");
                Console.WriteLine();

                PrintBoard();

                Console.WriteLine();
                Console.WriteLine("Comandos:");
                Console.WriteLine("- Ingresa: FILA COLUMNA NUMERO (ej: 1 1 5)");
                Console.WriteLine("- 'G' para Guardar");
                Console.WriteLine("- 'M' para volver al Menu");
                Console.WriteLine();
                Console.Write("Tu jugada: ");

                string? input = Console.ReadLine();
                if (input == null) input = "";
                input = input.ToUpper();

                if (input == "M")
                {
                    return;
                }
                else if (input == "G")
                {
                    SaveGame();
                    Console.WriteLine("Juego guardado! Presiona cualquier tecla...");
                    Console.ReadKey();
                }
                else if (input == "A")
                {
                    AutoComplete();
                }
                else
                {
                    ProcessMove(input);
                }
            }

            Console.Clear();
            PrintBoard();
            Console.WriteLine();

            if (gameWon)
            {
                Console.WriteLine("===============================");
                Console.WriteLine("  FELICIDADES! HAS GANADO!");
                Console.WriteLine("===============================");
            }
            else if (gameLost)
            {
                Console.WriteLine("===============================");
                Console.WriteLine("   GAME OVER - HAS PERDIDO");
                Console.WriteLine("===============================");
                Console.WriteLine();
                Console.Write("Jugar de nuevo? (S/N): ");
                string? response = Console.ReadLine();
                if (response == null) response = "";
                if (response.ToUpper() == "S")
                {
                    StartNewGame(currentDifficulty);
                    return;
                }
            }

            Console.WriteLine();
            Console.WriteLine("Presiona cualquier tecla para volver al menu...");
            Console.ReadKey();
        }

        static void ProcessMove(string input)
        {
            string[] parts = input.Split(' ');

            if (parts.Length != 3)
            {
                Console.WriteLine("Formato invalido. Presiona cualquier tecla...");
                Console.ReadKey();
                return;
            }

            int row, col, num;
            
            if (!int.TryParse(parts[0], out row) ||
                !int.TryParse(parts[1], out col) ||
                !int.TryParse(parts[2], out num))
            {
                Console.WriteLine("Debes ingresar numeros validos. Presiona cualquier tecla...");
                Console.ReadKey();
                return;
            }

            row--;
            col--;

            if (row < 0 || row > 8 || col < 0 || col > 8 || num < 0 || num > 9)
            {
                Console.WriteLine("Numeros fuera de rango (1-9). Presiona cualquier tecla...");
                Console.ReadKey();
                return;
            }

            if (initialBoard[row, col] != 0)
            {
                Console.WriteLine("No puedes modificar una celda inicial. Presiona cualquier tecla...");
                Console.ReadKey();
                return;
            }

            board[row, col] = num;

            if (solution[row, col] != num && num != 0)
            {
                mistakes++;

                if (mistakes >= 3)
                {
                    gameLost = true;
                }
            }

            if (num != 0 && !gameLost)
            {
                CheckWin();
            }
        }

        static void CheckWin()
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (board[i, j] != solution[i, j])
                    {
                        return;
                    }
                }
            }
            gameWon = true;
        }

        static void PrintBoard()
        {
            Console.WriteLine("    1 2 3   4 5 6   7 8 9");
            Console.WriteLine("  +-------+-------+-------+");

            for (int i = 0; i < 9; i++)
            {
                Console.Write((i + 1) + " | ");

                for (int j = 0; j < 9; j++)
                {
                    if (board[i, j] == 0)
                    {
                        Console.Write(". ");
                    }
                    else
                    {
                        Console.Write(board[i, j] + " ");
                    }

                    if ((j + 1) % 3 == 0)
                    {
                        Console.Write("| ");
                    }
                }

                Console.WriteLine();

                if ((i + 1) % 3 == 0 && i < 8)
                {
                    Console.WriteLine("  +-------+-------+-------+");
                }
            }

            Console.WriteLine("  +-------+-------+-------+");
        }

        static int[,] GenerateSolvedBoard()
        {
            int[,] newBoard = new int[9, 9];
            FillBoard(newBoard);
            return newBoard;
        }

        static bool FillBoard(int[,] board)
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (board[i, j] == 0)
                    {
                        List<int> numbers = new List<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                        Shuffle(numbers);

                        foreach (int num in numbers)
                        {
                            if (IsValid(board, i, j, num))
                            {
                                board[i, j] = num;

                                if (FillBoard(board))
                                {
                                    return true;
                                }

                                board[i, j] = 0;
                            }
                        }

                        return false;
                    }
                }
            }

            return true;
        }

        static bool IsValid(int[,] board, int row, int col, int num)
        {
            for (int x = 0; x < 9; x++)
            {
                if (board[row, x] == num)
                {
                    return false;
                }
            }

            for (int x = 0; x < 9; x++)
            {
                if (board[x, col] == num)
                {
                    return false;
                }
            }

            int startRow = row - (row % 3);
            int startCol = col - (col % 3);

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (board[i + startRow, j + startCol] == num)
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        static int[,] CreatePuzzle(int[,] solvedBoard, int cellsToRemove)
        {
            int[,] puzzle = new int[9, 9];
            
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    puzzle[i, j] = solvedBoard[i, j];
                }
            }
            
            Random random = new Random();
            int removed = 0;

            while (removed < cellsToRemove)
            {
                int row = random.Next(0, 9);
                int col = random.Next(0, 9);

                if (puzzle[row, col] != 0)
                {
                    puzzle[row, col] = 0;
                    removed++;
                }
            }

            return puzzle;
        }

        static void Shuffle(List<int> list)
        {
            Random random = new Random();
            int n = list.Count;

            while (n > 1)
            {
                n--;
                int k = random.Next(n + 1);
                int value = list[k];
                list[k] = list[n];
                list[n] = value;
            }
        }

        static void SaveGame()
        {
            try
            {
                using (StreamWriter writer = new StreamWriter("sudoku_save.txt"))
                {
                    writer.WriteLine(currentDifficulty);
                    writer.WriteLine(mistakes);
                    
                    for (int i = 0; i < 9; i++)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            writer.Write(board[i, j] + " ");
                        }
                        writer.WriteLine();
                    }
                    
                    for (int i = 0; i < 9; i++)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            writer.Write(solution[i, j] + " ");
                        }
                        writer.WriteLine();
                    }
                    
                    for (int i = 0; i < 9; i++)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            writer.Write(initialBoard[i, j] + " ");
                        }
                        writer.WriteLine();
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al guardar: " + ex.Message);
            }
        }

        static void LoadGame()
        {
            try
            {
                if (!File.Exists("sudoku_save.txt"))
                {
                    Console.WriteLine("No hay juego guardado. Presiona cualquier tecla...");
                    Console.ReadKey();
                    return;
                }

                using (StreamReader reader = new StreamReader("sudoku_save.txt"))
                {
                    string? line = reader.ReadLine();
                    currentDifficulty = line ?? "1";
                    line = reader.ReadLine();
                    mistakes = int.Parse(line ?? "0");
                    
                    for (int i = 0; i < 9; i++)
                    {
                        line = reader.ReadLine();
                        if (line == null) continue;
                        string[] values = line.Split(' ');
                        for (int j = 0; j < 9; j++)
                        {
                            board[i, j] = int.Parse(values[j]);
                        }
                    }
                    
                    for (int i = 0; i < 9; i++)
                    {
                        line = reader.ReadLine();
                        if (line == null) continue;
                        string[] values = line.Split(' ');
                        for (int j = 0; j < 9; j++)
                        {
                            solution[i, j] = int.Parse(values[j]);
                        }
                    }
                    
                    for (int i = 0; i < 9; i++)
                    {
                        line = reader.ReadLine();
                        if (line == null) continue;
                        string[] values = line.Split(' ');
                        for (int j = 0; j < 9; j++)
                        {
                            initialBoard[i, j] = int.Parse(values[j]);
                        }
                    }
                }

                gameWon = false;
                gameLost = false;
                PlayGame();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al cargar: " + ex.Message);
                Console.ReadKey();
            }
        }

        static void AutoComplete()
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    board[i, j] = solution[i, j];
                }
            }
            gameWon = true;
        }
    }
}