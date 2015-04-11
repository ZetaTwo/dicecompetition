var settings = {
	WIDTH: 100,
	HEIGHT: 100
};

var game = {

	playarea: null,

	init: function() {
		this.playarea = $('#grid');
		for(var y = 0; y < settings.HEIGHT; y++) {
			var newrow = $('<tr id="row_"' + y + '>');
			for(var x = 0; x < settings.HEIGHT; x++) {
				newrow.append('<td id="cell_' + y + '_' + x + '">');
			}

			this.playarea.append(newrow);

		}
		//this.playarea
	}
};

jQuery(document).ready(function() {
	game.init();
});