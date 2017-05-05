%{
	Representação binária da parte fracionária de 1/d
	Opcional:
		str - Se for verdadeiro, retorna a represetação em string
		em vez de array
%}

function s = bin(d, str)
	n = 1/d;
    s = [];
    for i = 1:64
        n = 2*n;
        f = fix(n);
        s(end+1) = f;
        n = n - f;
    end
	% Retorna string
	if (nargin == 2 && str)
		s = char('0' + s);
	end
end
