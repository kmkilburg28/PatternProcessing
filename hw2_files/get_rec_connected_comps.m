function final_list = get_rec_connected_comps(connected_comps, equivTable)
    rec_list = [];
    for i = 1:size(connected_comps)
        nextEquiv = connected_comps(i);
        rec_list = [rec_list, nextEquiv];
        if (equivTable(connected_comps(i),end) > 0)
            rec_list = [rec_list, get_rec_connected_comps( ...
                equivTable(nextEquiv,1:equivTable(nextEquiv,end)), ...
                equivTable)];
        end
    end
    final_list = rec_list;
end