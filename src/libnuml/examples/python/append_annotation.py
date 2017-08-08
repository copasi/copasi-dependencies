import sys

import libnuml


def add_annotation(input_file, output_file, meta_id, annotation):
    # type: (str, str, str, str) -> None
    """

    :param input_file: the input numl file
    :param output_file: the output numl file
    :param meta_id: the id to which to attach the annotation
    :param annotation: the annotation

    """

    doc = libnuml.readNUMLFromFile(input_file)
    assert(isinstance(doc, libnuml.NUMLDocument))
    added = add_annotation_for_metaid(doc, meta_id, annotation)

    if not added:
        for i in range(doc.getNumResultComponents()):
            current = doc.getResultComponent(i)
            assert(isinstance(current, libnuml.ResultComponent))
            if add_annotation_for_metaid(current, meta_id, annotation):
                added = True
                break

    libnuml.writeNUML(doc, output_file)


def add_annotation_for_metaid(current, meta_id, annotation):
    # type: (libnuml.NMBase, str, str) -> bool
    """
    Adds the annotation if the meta id matches the element
    :param current: the current element
    :param meta_id: the meta id to use for the annotation
    :param annotation: the annotation
    :return: True, if annotation was set, False otherwise
    """
    if current.getMetaId() == meta_id:
        current.setAnnotation(annotation)
        return True
    return False


if __name__ == '__main__':
    if len(sys.argv) < 5:
        print('Need four arguments input-file, output-file, id, annotation')
        sys.exit(1)

    add_annotation(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
