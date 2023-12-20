import sys
from typing import Generator

class Solver:
    def __init__(self):
        self.filepath = "input.txt"

    def decode_line(self, line: str) -> tuple[int, list[str]]:
        """
        Decode line to get game id and draw details for the game.
        """
        game_detail, draw_details = line.split(":")
        return (int(game_detail.split()[-1]), draw_details.split(";"))

    def decode_draw(
        self, game_set: str
    ) -> Generator[tuple[int, int], None, None]:
        """
        Decodes set details to a list of color id, number of color pairs.
        """
        for ball_details in game_set.split(", "):
            num_token, color_token = ball_details.split()
            yield int(
                num_token
            ), 0 if color_token == "red" else 1 if color_token == "green" else 2

    def part_2(self):
        def get_min_colors(draws: list[str]) -> list[int]:
            colors = [0, 0, 0]
            for draw in draws:
                for num, color in self.decode_draw(draw):
                    colors[color] = max(colors[color], num)
            return colors

        def multiply(colors: list[int]) -> int:
            result = 1
            for x in colors:
                result *= x
            return result

        with open(
            self.filepath, "r", encoding=sys.getdefaultencoding()
        ) as file:
            return sum(
                multiply(get_min_colors(draws))
                for _, draws in (self.decode_line(line) for line in file)
            )

    def solve(self) -> None:
        print("Part 2:")
        print(self.part_2())


if __name__ == "__main__":
    solver = Solver()
    solver.solve()
