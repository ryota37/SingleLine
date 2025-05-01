# include <Siv3D.hpp>

void UpdateGrid(Grid<int32>& grid)
{
	// クリックされていない場合は何もしない
	if (not MouseL.down())
	{
		return;
	}

	for (int32 y = 0; y < grid.height(); ++y)
	{
		for (int32 x = 0; x < grid.width(); ++x)
		{
			const RectF rect{ (x * 100), (y * 100), 100 };

			if (rect.mouseOver())
			{
				// クリックのたびに要素を 0 → 1 → 2 → 3 → 0 → 1 → ... と変化させる
				++grid[y][x] %= 4;
			}
		}
	}
}

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

void UpdateCircle(Circle& circle)
{
	if (KeyLeft.down()) circle.x -= 100;
	if (KeyUp.down()) circle.y -= 100;
	if (KeyRight.down()) circle.x += 100;
	if (KeyDown.down()) circle.y += 100;
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
				// 円が格子を通るたびに要素を 0 → 1 → 2 → 3 → 0 → 1 → ... と変化させる
				++grid[y][x] %= 4;
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
		UpdateGrid(grid);

		DrawGrid(grid);

		UpdateCircle(circle);

		DrawCircle(circle);

		VisualizeCircleRoute(circle, grid);

		PrintGoal(circle);
	}
}
