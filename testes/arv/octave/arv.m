function f = arv(gs)
	global GRAUS = [2 2 2];
	global graus alt nums fols;
	
	if (nargin == 0)
		graus = GRAUS;
	else
		graus = gs;
	end
	% Altura da árvore
	alt = length(graus);

	% Cria todos os nós
	nums = [];
	fols = [];
	ad(0, 1, 0);

	% Mostra os números
	if nargout == 0
		fprintf("Todos:\n");
		disp(dec2bin(nums));
		fprintf("\nFolhas:\n");
		disp(dec2bin(fols));
	end

	if (nargout == 1) f = fols; end
end