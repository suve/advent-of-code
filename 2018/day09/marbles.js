#!/usr/local/bin/node
let fs = require('fs');


function thegame(player_num, marble_num) {
	let score = [];
	for(let p = 0; p < player_num; ++p) score[p] = 0;
	let player = 0;
	
	let marbles = [0];
	let current = 0;
	
	for(let m = 1; m <= marble_num; ++m) {
		// console.log(marbles.join());
		
		if(m % 23 == 0) {
			player = (player + 23) % player_num;
			score[player] += m;
			
			let remove_pos = current - 7;
			if(remove_pos < 0) remove_pos += marbles.length;
			
			let removed = marbles.splice(remove_pos, 1);
			score[player] += removed[0];
			
			current = remove_pos % marbles.length;
		} else {
			let insert_pos = (current + 2) % (marbles.length);
			marbles.splice(insert_pos, 0, m);
			current = insert_pos;
		}
	}
	
	let best = 0;
	for(let p = 0; p < player_num; ++p) {
		console.log('player #' + p + ' score: ' + score[p]);
		if(score[p] > best) best = score[p];
	}
	console.log('--> best score: ' + best);
}


let inputs = fs.readFileSync('/dev/stdin').toString().split('\n');

for(let i = 0; i < inputs.length; ++i) {
	let line = inputs[i].trim().split(';');
	if(line.length < 2) continue;
	
	let players = line[0].replace(/[^0-9]/g, '');
	let marbles = line[1].replace(/[^0-9]/g, '');
	
	thegame(Number(players), Number(marbles));
}
