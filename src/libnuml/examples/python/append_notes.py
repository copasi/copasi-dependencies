import sys

import libnuml


def add_notes(input_file, output_file, meta_id, notes):
    # type: (str, str, str, str) -> None
    """

    :param input_file: the input numl file
    :param output_file: the output numl file
    :param meta_id: the id to which to attach the notes
    :param notes: the notes

    """

    doc = libnuml.readNUMLFromFile(input_file)
    assert(isinstance(doc, libnuml.NUMLDocument))
    added = add_notes_for_metaid(doc, meta_id, notes)

    if not added:
        for i in range(doc.getNumResultComponents()):
            current = doc.getResultComponent(i)
            assert(isinstance(current, libnuml.ResultComponent))
            if add_notes_for_metaid(current, meta_id, notes):
                added = True
                break

    libnuml.writeNUML(doc, output_file)


def add_notes_for_metaid(current, meta_id, notes):
    # type: (libnuml.NMBase, str, str) -> bool
    """
    Adds the notes if the meta id matches the element
    :param current: the current element
    :param meta_id: the meta id to use for the notes
    :param notes: the notes
    :return: True, if notes was set, False otherwise
    """
    if current.getMetaId() == meta_id:
        current.setNotes(notes)
        return True
    return False


if __name__ == '__main__':
    if len(sys.argv) < 5:
        print('Need four arguments input-file, output-file, id, notes')
        sys.exit(1)

    add_notes(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
