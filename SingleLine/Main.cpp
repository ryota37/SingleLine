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

bool isWallUp(Circle& circle, Grid<int32>& grid)
{
	int grid_x = static_cast<int>(circle.x) / 100;
	int grid_y = static_cast<int>(circle.y) / 100;
	int grid_y_up = std::max(grid_y - 1, 0);

	if (circle.y == 50) return true;
	if (grid[grid_y_up][grid_x] > 0) return true;

	return false;
}

bool isWallLeft(Circle& circle, Grid<int32>& grid)
{
	int grid_x = static_cast<int>(circle.x) / 100;
	int grid_y = static_cast<int>(circle.y) / 100;
	int grid_x_left = std::max(grid_x - 1, 0);

	if (circle.x == 50) return true;
	if (grid[grid_y][grid_x_left] > 0) return true;

	return false;
}

bool isWallDown(Circle& circle, Grid<int32>& grid)
{
	int grid_x = static_cast<int>(circle.x) / 100;
	int grid_y = static_cast<int>(circle.y) / 100;
	int grid_y_down = std::min(grid_y + 1, 7);

	if (circle.y == 550) return true;
	if (grid[grid_y_down][grid_x] > 0) return true;

	return false;
}

bool isWallRight(Circle& circle, Grid<int32>& grid)
{
	int grid_x = static_cast<int>(circle.x) / 100;
	int grid_y = static_cast<int>(circle.y) / 100;
	int grid_x_right = std::min(grid_x + 1, 5);

	if (circle.x == 750) return true;
	if (grid[grid_y][grid_x_right] > 0) return true;

	return false;
}

void UpdateCircle(Circle& circle, Grid<int32>& grid)
{
	if (KeyLeft.down() && !isWallLeft(circle, grid)) circle.x -= 100;
	if (KeyUp.down() && !isWallUp(circle, grid)) circle.y -= 100;
	if (KeyRight.down() && !isWallRight(circle, grid)) circle.x += 100;
	if (KeyDown.down() && !isWallDown(circle, grid)) circle.y += 100;
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
