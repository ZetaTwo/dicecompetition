var settings = {
	WIDTH: 100,
	HEIGHT: 100
};

var game = {

	playarea: null,
	level: 1,

	player: {
		x: 0,
		y: 0
	},
	robots: [],
	scrap: [],
	winner: 0,

	init: function() {
		this.playarea = $('#grid');
		for(var y = 0; y < settings.HEIGHT; y++) {
			var newrow = $('<tr id="row_"' + y + '>');
			for(var x = 0; x < settings.HEIGHT; x++) {
				newrow.append('<td class="cell" id="cell_' + y + '_' + x + '">');
			}
			this.playarea.append(newrow);
		}

		self = this;
		$('#move1').click(function() { self.move(1); });
		$('#move2').click(function() { self.move(2); });
		$('#move3').click(function() { self.move(3); });
		$('#move4').click(function() { self.move(4); });
		$('#move5').click(function() { self.move(5); });
		$('#move6').click(function() { self.move(6); });
		$('#move7').click(function() { self.move(7); });
		$('#move8').click(function() { self.move(8); });
		$('#move9').click(function() { self.move(9); });
	},

	process_data: function(data) {
		this.player = data.player;
		this.robots = data.robots;
		this.scrap = data.scrap;
	},

	startgame: function(level) {
		$.post('/restart', { level: level}, function(data) {

			self.process_data(data);
			self.updategrid();
			self.updatecontrols();
		}, 'json');
	},

	updatecontrols: function() {
		$('.btn_move').addClass('off');

		if(this.player.x > 0) $('#move4').removeClass('off');
		if(this.player.x + 1< settings.WIDTH) $('#move6').removeClass('off');
		if(this.player.y > 0) $('#move2').removeClass('off');
		if(this.player.y + 1 < settings.HEIGHT) $('#move8').removeClass('off');

		if(this.player.x > 0 && this.player.y > 0) $('#move1').removeClass('off');
		if(this.player.x + 1 < settings.WIDTH && this.player.y > 0) $('#move3').removeClass('off');
		if(this.player.x > 0 && this.player.y + 1 < settings.HEIGHT) $('#move7').removeClass('off');
		if(this.player.x + 1 < settings.WIDTH && this.player.y + 1 < settings.HEIGHT) $('#move9').removeClass('off');
	},

	updategrid: function() {
		if(this.winner != 0) {
			this.endgame();
			return;
		}

		$('.cell').removeClass('robot player scrap');
		for(var i = 0; i < this.robots.length; i++) {
			$('#cell_' + this.robots[i][1] + '_' + this.robots[i][0]).addClass('robot');
		}
		for(var i = 0; i < this.scrap.length; i++) {
			$('#cell_' + this.scrap[i][1] + '_' + this.scrap[i][0]).addClass('scrap');
		}
		$('#cell_' + this.player[1] + '_' + this.player[0]).addClass('player');
	},

	endgame: function() {
		if(this.winner == 1) {
			$('#win').removeClass('hide');
			setTimeout(function() {
				$('#win').addClass('hide');
			}, 5000);
			this.level += 1;
			this.startgame(this.level);
		} else if(this.winner == 2) {
			$('#lose').removeClass('hide');
		}
	},

	move: function(dir) {
		self = this;
		$.post('/restart', { move: dir}, function(data) {
			self.process_data(data);
			self.updategrid();
			self.updatecontrols();
		}, 'json');
	}
};

jQuery(document).ready(function() {
	game.init();
	game.startgame(1);
});