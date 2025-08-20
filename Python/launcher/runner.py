#!/usr/bin/env python3
"""
Comprehensive Python launcher for Advent of Code solutions.
This launcher discovers and runs all Python solutions in the year2022 directory.
"""

import os
import sys
import importlib
import subprocess
import time
from pathlib import Path
from typing import List, Optional

# Add the libs directory to the Python path
script_dir = Path(__file__).parent
project_root = script_dir.parent
libs_path = project_root / "libs"
sys.path.insert(0, str(libs_path))

from aoc_libs.utils.ISolution import ISolution
from inspect import getmembers, isclass


class SolutionRunner:
    def __init__(self, year_dir: Path):
        self.year_dir = year_dir
        self.results = []
        
    def discover_solutions(self) -> List[tuple]:
        """Discover all solutions in the year directory."""
        solutions = []
        
        # Find all day directories
        day_dirs = sorted([d for d in self.year_dir.iterdir() if d.is_dir() and d.name.startswith("Day")])
        
        for day_dir in day_dirs:
            day_num = day_dir.name[3:]  # Extract number from "DayXX"
            
            # Look for Python files in the day directory
            py_files = list(day_dir.glob("*.py"))
            
            for py_file in py_files:
                module_name = py_file.stem
                relative_path = f"Day{day_num}.{module_name}"
                
                try:
                    # Try to import and check for ISolution classes
                    module = importlib.import_module(relative_path)
                    
                    # Look for classes that implement ISolution
                    solution_classes = []
                    for name, obj in getmembers(module):
                        if isclass(obj) and issubclass(obj, ISolution) and obj != ISolution:
                            solution_classes.append((name, obj))
                    
                    if solution_classes:
                        # Add ISolution-based solutions
                        for class_name, class_obj in solution_classes:
                            solutions.append(('class', day_num, relative_path, class_name, class_obj))
                    else:
                        # Add script-based solutions
                        solutions.append(('script', day_num, str(py_file), module_name, None))
                        
                except ImportError as e:
                    print(f"Warning: Could not import {relative_path}: {e}")
                    # Fall back to script execution
                    solutions.append(('script', day_num, str(py_file), module_name, None))
                except Exception as e:
                    print(f"Warning: Error processing {relative_path}: {e}")
        
        return solutions
    
    def run_class_solution(self, class_name: str, class_obj, day_num: str) -> Optional[str]:
        """Run a class-based solution that implements ISolution."""
        try:
            print(f"year2022.Day{day_num}.{class_name}")
            instance = class_obj()
            
            # Capture Part1 output
            original_stdout = sys.stdout
            from io import StringIO
            part1_output = StringIO()
            sys.stdout = part1_output
            try:
                instance.Part1()
            finally:
                sys.stdout = original_stdout
            part1_result = part1_output.getvalue().strip()
            
            # Capture Part2 output
            part2_output = StringIO()
            sys.stdout = part2_output
            try:
                instance.Part2()
            finally:
                sys.stdout = original_stdout
            part2_result = part2_output.getvalue().strip()
            
            # Format output like Java/C++ solutions
            output = []
            if part1_result:
                # Extract the actual result (assume format like "Part 1: 12345")
                if "Part 1:" in part1_result:
                    result = part1_result.split("Part 1:")[-1].strip()
                else:
                    result = part1_result
                output.append(f"\tSolution 1: {result}")
            
            if part2_result:
                # Extract the actual result (assume format like "Part 2: 67890")
                if "Part 2:" in part2_result:
                    result = part2_result.split("Part 2:")[-1].strip()
                else:
                    result = part2_result
                output.append(f"\tSolution 2: {result}")
            
            result_text = "\n".join(output) + "\n"
            print(result_text, end="")
            return result_text
            
        except Exception as e:
            error_msg = f"\tError: {str(e)}\n"
            print(error_msg, end="")
            return error_msg
    
    def run_script_solution(self, script_path: str, day_num: str, module_name: str) -> Optional[str]:
        """Run a script-based solution."""
        try:
            print(f"year2022.Day{day_num}.{module_name}")
            
            # Change to the year directory so relative paths work
            original_cwd = os.getcwd()
            os.chdir(self.year_dir)
            
            try:
                # Run the script and capture output
                result = subprocess.run([sys.executable, script_path], 
                                      capture_output=True, text=True, timeout=60)
                
                if result.returncode == 0:
                    output_lines = result.stdout.strip().split('\n')
                    formatted_output = []
                    
                    for line in output_lines:
                        if line.strip():
                            if "Part 1:" in line:
                                result_val = line.split("Part 1:")[-1].strip()
                                formatted_output.append(f"\tSolution 1: {result_val}")
                            elif "Part 2:" in line:
                                result_val = line.split("Part 2:")[-1].strip()
                                formatted_output.append(f"\tSolution 2: {result_val}")
                            elif not line.startswith('\t'):
                                # Assume it's a direct result if no "Part X:" prefix
                                if len(output_lines) == 2:  # Two lines likely means Part 1 and Part 2
                                    idx = output_lines.index(line)
                                    formatted_output.append(f"\tSolution {idx + 1}: {line.strip()}")
                                else:
                                    formatted_output.append(f"\t{line.strip()}")
                    
                    if not formatted_output and result.stdout.strip():
                        # If we couldn't parse it, just include the raw output
                        formatted_output.append(f"\t{result.stdout.strip()}")
                    
                    result_text = "\n".join(formatted_output) + "\n"
                    print(result_text, end="")
                    return result_text
                else:
                    error_msg = f"\tError: {result.stderr.strip()}\n"
                    print(error_msg, end="")
                    return error_msg
                    
            finally:
                os.chdir(original_cwd)
                
        except subprocess.TimeoutExpired:
            error_msg = f"\tError: Timeout after 60 seconds\n"
            print(error_msg, end="")
            return error_msg
        except Exception as e:
            error_msg = f"\tError: {str(e)}\n"
            print(error_msg, end="")
            return error_msg
    
    def run_all_solutions(self) -> str:
        """Run all discovered solutions and return formatted results."""
        solutions = self.discover_solutions()
        all_results = []
        
        for solution_type, day_num, path_or_module, name, class_obj in solutions:
            try:
                if solution_type == 'class':
                    result = self.run_class_solution(name, class_obj, day_num)
                else:
                    result = self.run_script_solution(path_or_module, day_num, name)
                
                if result:
                    all_results.append(result)
                    
            except Exception as e:
                error_result = f"year2022.Day{day_num}.{name}\n\tError: {str(e)}\n"
                print(error_result, end="")
                all_results.append(error_result)
        
        return "".join(all_results)


def main():
    start_time = time.time()
    
    # Find the year2022 directory
    script_dir = Path(__file__).parent
    year_dir = script_dir.parent / "2022" / "main" / "year2022"
    
    if not year_dir.exists():
        print(f"Error: Year directory not found at {year_dir}")
        sys.exit(1)
    
    # Change to the year directory for relative imports
    original_sys_path = sys.path.copy()
    sys.path.insert(0, str(year_dir))
    
    try:
        runner = SolutionRunner(year_dir)
        results = runner.run_all_solutions()
        
        # Write results to file for comparison
        results_file = year_dir / "res.log"
        with open(results_file, "w") as f:
            f.write(results)
        
        # Write timing information
        end_time = time.time()
        exception_file = year_dir / "exception.log"
        with open(exception_file, "w") as f:
            f.write(f"Cost: {(end_time - start_time) * 1000:.6f}\n")
        
        print(f"\nResults saved to {results_file}")
        print(f"Execution time: {(end_time - start_time) * 1000:.6f} ms")
        
    finally:
        sys.path = original_sys_path


if __name__ == "__main__":
    main()