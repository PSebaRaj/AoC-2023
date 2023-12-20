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

    def part_1(self):
        max_colors = [12, 13, 14]

        def all_draws_less_than_max(draws: list[str]) -> bool:
            def draw_less_than_max(draw: str) -> bool:
                return all(
                    num <= max_colors[color]
                    for num, color in self.decode_draw(draw)
                )

            return all(draw_less_than_max(draw) for draw in draws)

        with open(
            self.filepath, "r", encoding=sys.getdefaultencoding()
        ) as file:
            return sum(
                game_id
                for game_id, draws in (self.decode_line(line) for line in file)
                if all_draws_less_than_max(draws)
            )

    def solve(self) -> None:
        print("Part 1:")
        print(self.part_1())


if __name__ == "__main__":
    solver = Solver()
    solver.solve()
