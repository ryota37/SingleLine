# include <Siv3D.hpp>

void DrawGrid(const Grid<int32>& grid)
{
	for (int32 y = 0; y < grid.height(); ++y)
	{
		for (int32 x = 0; x < grid.width(); ++x)
		{
			const RectF rect{ (x * 100), (y * 100), 100 };
			const ColorF color{ (3 - grid[y][x]) / 3.0 };
			rect.stretched(-1).draw(color);
		}
	}
}

void DrawCircle(const Circle& circle)
{
	circle.draw(Palette::Aqua);
}

enum class Direction {Up, Down, Left, Right};

bool isWallUp(Circle& circle, Grid<int32>& grid, int grid_x, int grid_y)
{
	if (circle.y == 50) return true;
	if (grid[std::max(grid_y - 1, 0)][grid_x] > 0) return true;
	return false;
}

bool isWallLeft(Circle& circle, Grid<int32>& grid, int grid_x, int grid_y)
{
	if (circle.x == 50) return true;
	if (grid[grid_y][std::max(grid_x - 1, 0)] > 0) return true;
	return false;
}

bool isWallDown(Circle& circle, Grid<int32>& grid, int grid_x, int grid_y)
{
	if (circle.y == 550) return true;
	if (grid[std::min(grid_y + 1, 7)][grid_x] > 0) return true;
	return false;
}

bool isWallRight(Circle& circle, Grid<int32>& grid, int grid_x, int grid_y)
{
	if (circle.x == 750) return true;
	if (grid[grid_y][std::min(grid_x + 1, 5)] > 0) return true;
	return false;
}

bool isWall(Circle& circle, Grid<int32>& grid, Direction dir)
{
	int grid_x = static_cast<int>(circle.x) / 100;
	int grid_y = static_cast<int>(circle.y) / 100;

	switch (dir)
	{
	case Direction::Up: return isWallUp(circle, grid, grid_x, grid_y);
	case Direction::Left: return isWallLeft(circle, grid, grid_x, grid_y);
	case Direction::Down: return isWallDown(circle, grid, grid_x, grid_y);
	case Direction::Right: return isWallRight(circle, grid, grid_x, grid_y);
	}
	return false;
}

void UpdateCircle(Circle& circle, Grid<int32>& grid)
{
	if (KeyUp.down() && !isWall(circle, grid, Direction::Up)) circle.y -= 100;
	if (KeyLeft.down() && !isWall(circle, grid, Direction::Left)) circle.x -= 100;
	if (KeyDown.down() && !isWall(circle, grid, Direction::Down)) circle.y += 100;
	if (KeyRight.down() && !isWall(circle, grid, Direction::Right)) circle.x += 100;
}

void PrintGoal(const Circle& circle)
{
	if (circle.x != 750) return;
	if (circle.y != 550) return;
	Print << U"Goal!!!";
}

bool isDirectionKeyPressed()
{
	if (KeyLeft.down()) return true;
	if (KeyUp.down()) return true;
	if (KeyRight.down()) return true;
	if (KeyDown.down()) return true;
	return false;
}


void VisualizeCircleRoute(const Circle& circle, Grid<int32>& grid)
{
	int grid_x = static_cast<int>(circle.x) / 100;
	int grid_y = static_cast<int>(circle.y) / 100;

	for (int32 y = 0; y < grid.height(); ++y)
	{
		for (int32 x = 0; x < grid.width(); ++x)
		{
			const RectF rect{ (x * 100), (y * 100), 100 };
			if (x == grid_x && y == grid_y && isDirectionKeyPressed())
			{
				// 円が格子を通るたびに要素を 0 → 1 → 0 → 1 ... と変化させる
				++grid[y][x] %= 2;
			}
		}
	}
}

void Main()
{
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	// 8x6 の二次元配列を作成し、全ての要素を 0 で初期化する
	Grid<int32> grid(8, 6);

	Circle circle{ 50,50,25 };

	while (System::Update())
	{
		DrawGrid(grid);

		UpdateCircle(circle, grid);

		DrawCircle(circle);

		VisualizeCircleRoute(circle, grid);

		PrintGoal(circle);
	}
}
