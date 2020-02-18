use std::clone::Clone;
use std::cmp::Eq;
use std::cmp::PartialEq;
use std::convert::TryInto;
use std::io;
use std::io::Write;
use std::process;
use std::vec;

struct Area {
	rows: usize,
	columns: usize,
	data: Vec<u8>
}

impl Clone for Area {
    fn clone(&self) -> Self {
        let capacity = self.rows * self.columns;
        let mut datacopy: Vec<u8> = Vec::with_capacity(capacity);
        for i in 0..capacity {
            datacopy.push(self.data[i]);
        }

        return Area { rows: self.rows, columns: self.columns, data: datacopy };
    }
}

impl PartialEq for Area {
    fn eq(&self, other: &Self) -> bool {
        if (self.rows != other.rows) || (self.columns != other.columns) {
            return false;
        }

        for i in 0..(self.rows * self.columns) {
            if self.data[i] != other.data[i] {
                return false;
            }
        }

        return true;
    }
}

impl Eq for Area { }

fn string_to_ints(string: &str) -> Vec<usize> {
    let mut result: Vec<usize> = Vec::new();

    for fragment in string.trim().split_ascii_whitespace()	{
        let value: usize = fragment.parse().expect("Failed to parse integer value");
        result.push(value);
    }

    return result;
}

fn read_stdin() -> Area {
	let mut line = String::new();
	let ok = io::stdin().read_line(&mut line);
    if !ok.is_ok() {
        eprintln!("Failed to read Row #0 of input: {}", ok.unwrap_err());
        process::exit(1);
    }

    let size = string_to_ints(&line);
    if size.len() < 2 {
		eprintln!("Row #0 of input should contain number of rows and columns");
        process::exit(1);
    }
    let rows = size[0];
    let columns = size[1];

	let mut data: Vec<u8> = Vec::with_capacity(rows * columns);
	for y in 0..rows {
        line.clear();
		let ok = io::stdin().read_line(&mut line);
        if !ok.is_ok() {
			eprintln!("Failed to read Row #{} of input: {}", (y+1), ok.unwrap_err());
            process::exit(1);
        }

        let bytes = line.trim().as_bytes();
		if bytes.len() != columns {
			eprintln!("Expected {} bytes in input row, but got {}", columns, bytes.len());
            process::exit(1);
		}

		data.append(&mut bytes.to_vec());
	}

    if data.len() != rows*columns {
        eprintln!("Expected {} bytes in input total, but got {}", rows*columns, data.len());
        process::exit(1);
    }

	return Area{ rows: rows, columns: columns, data: data };
}

fn get_tile(data: &Area, x: usize, y: usize) -> u8 {
	if (x >= data.columns) || (y >= data.rows) { 
		return 'x' as u8;
	}

	return data.data[(y * data.columns) + x];
}

fn get_tile_i(data: &Area, x: isize, y: isize) -> u8 {
	if (x < 0) || (y < 0) {
		return 'x' as u8;
	}

    return get_tile(data, x.try_into().unwrap(), y.try_into().unwrap());
}


fn step(data: &Area) -> Area {
	let mut result: Vec<u8> = Vec::with_capacity(data.rows * data.columns);
	
	for y in 0..data.rows {
		for x in 0..data.columns {
			let (mut trees, mut lumber) = (0, 0);
			
			for yd in 0..3 {
				for xd in 0..3 {
					let nx = (x as isize) + (xd as isize) - 1;
					let ny = (y as isize) + (yd as isize) - 1;
                    if (ny == y as isize) && (nx == x as isize) {
						continue;
					}

					match get_tile_i(&data, nx, ny) as char {
						'|' => trees += 1,
						'#' => lumber += 1,
						_ => ()
					}
				}
			}

			let mut tile = get_tile(&data, x, y);
			match tile as char {
				'.' => { if trees >= 3 { tile = '|' as u8; }}
				'|' => { if lumber >= 3 { tile = '#' as u8; }}
				'#' => { if (lumber == 0) || (trees == 0) { tile = '.' as u8; }}
				_ => ()
			}

			result.push(tile);
		}
	}

	return Area{ rows: data.rows, columns: data.columns, data: result };
}

fn print_area(a: &Area) {
	let mut out = io::stdout();

	for y in 0..a.rows {
		let start = y * a.columns;
		let end = (y+1) * a.columns;
		out.write(&a.data[start..end]);
		out.write(&['\n' as u8]);
	}
	
    out.write(&['\n' as u8]);
}

fn resource_value(data: &Area) -> u64 {
	let (mut trees, mut lumber) : (u64, u64) = (0, 0);
	for i in 0..(data.rows * data.columns) {
		match data.data[i] as char {
			'|' => trees += 1,
			'#' => lumber += 1,
			_ => ()
		}
	}

	return trees * lumber;
}

fn main() {
	let mut area = read_stdin();
	println!("Initially: {}", resource_value(&area));
    print_area(&area);

    let mut history: Vec<Area> = Vec::new();
    history.push(area.clone());

    let mut index: usize = 0;
    
    let mut time: usize = 0;
    const LIMIT: usize = 1000000000;
    while time < LIMIT {
        area = step(&area);
        time += 1;

        match history.iter().position(|elem| elem == &area) {
            Some(pos) => {
                let cycle_length = time - pos;
                eprintln!("Cycle of length {} found after {} steps", cycle_length, time);
                
                index = pos + ((LIMIT - time) % cycle_length);
                time = LIMIT;
            }
            None => {
                history.push(area.clone());
            }
        }
    }

    if index != 0 {
        area = history[index].clone();
    }

    print_area(&area);
    println!("Value: {}", resource_value(&area));
}
